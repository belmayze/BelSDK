/*!
 * @file   Program.cs
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
using CommandLine;
using System.Diagnostics;
using System.Text.Json;
using System.Text.RegularExpressions;

namespace ShaderConverter
{
    /// <summary>
    /// オプション
    /// </summary>
    class Options
    {
        [Option('i', "input", Required = true, HelpText = "入力")]
        public string Input { get; set; } = "";
        [Option('o', "output-dir", HelpText = "出力ディレクトリ")]
        public string? OutputDir { get; set; } = null;
    }

    /// <summary>
    /// ファイルデータ
    /// </summary>

    class FileData
    {
        public class SettingProperty
        {
            public string Profile { get; set; } = "5_1";
        }

        public class TargetProperty
        {
            //! シェーダー名
            public string Name { get; set; } = "";
            //! 頂点シェーダー
            public string? VertexShader { get; set; } = null;
            //! ジオメトリシェーダー
            public string? GeometryShader { get; set; } = null;
            //! ピクセルシェーダー
            public string? PixelShader { get; set; } = null;
            //! コンピュートシェーダー
            public string? ComputeShader { get; set; } = null;
            //! ドメインシェーダー
            public string? DomainShader { get; set; } = null;
            //! ハルシェーダー
            public string? HullShader { get; set; } = null;
            //! アンプリフィケーションシェーダー
            public string? AmplificationShader { get; set; } = null;
            //! メッシュシェーダー
            public string? MeshShader { get; set; } = null;
        }

        //! 設定
        public SettingProperty Setting { get; set; } = new SettingProperty();
        //! インクルードファイルリスト
        public List<string> Includes { get; set; } = new List<string>();
        //! シェーダーリスト
        public List<TargetProperty> Targets { get; set; } = new List<TargetProperty>();
    }

    class Program
    {
        static int Main(string[] args)
        {
            // 引数解析
            ParserResult<Options> parserResult = Parser.Default.ParseArguments<Options>(args);
            if (parserResult.Tag == ParserResultType.NotParsed)
            {
                return 1;
            }
            Options options = parserResult.Value;

            // 入力された json を読み込む
            FileData? fileData = null;
            try
            {
                FileStream stream = new FileStream(options.Input, FileMode.Open);

                try
                {
                    fileData = JsonSerializer.Deserialize<FileData>(stream);
                }
                catch (Exception e)
                {
                    Console.Error.WriteLine($"Error: {e.Message}");
                    return 2;
                }
            }
            catch (Exception e)
            {
                Console.Error.WriteLine($"Error: {e.Message}");
                return 2;
            }
            if (fileData is null) { return -3; }

            // 出力パスの決定
            string? outputDir = Path.GetDirectoryName(Path.GetFullPath(options.Input));
            if (outputDir is null) { return 1; }
            if (options.OutputDir is not null) { outputDir = options.OutputDir; }
            if (!Directory.Exists(outputDir)) { Directory.CreateDirectory(outputDir); }

            // ワーキングパス
            string workingPath = Environment.ExpandEnvironmentVariables("%TEMP%\\BelSDK\\ShaderConverter");
            if (!Directory.Exists(workingPath))
            {
                Directory.CreateDirectory(workingPath);
            }
            else
            {
                string[] files = Directory.GetFiles(workingPath, "*", SearchOption.AllDirectories);
                foreach (string file in files) { File.Delete(file); }
            }

            // include に含まれているファイルをすべてコピーする
            string? relativeRootPath = Path.GetDirectoryName(Path.GetFullPath(options.Input));
            if (relativeRootPath is null) { return 1; }

            foreach (string include in fileData.Includes)
            {
                string path = Environment.ExpandEnvironmentVariables(include);
                if (!Path.IsPathRooted(path)) { path = $"{relativeRootPath}\\{path}"; }

                // ファイルをコピーする
                string[] files = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly);
                foreach (string file in files) { File.Copy(file, $"{workingPath}\\{Path.GetFileName(file)}"); }
            }

            // Windows10 SDK のインストール先チェック
            string windowsBinDir = "";
            {
                // Windows.h が含まれているパスから SDK バージョンをチェック
                string[] windowsSdkDirRoots = { "C:\\Program Files (x86)\\Windows Kits\\10", "C:\\Program Files\\Windows Kits\\10" };
                string windowsSdkDirRoot = "";
                string windowsSdkVersion = "";
                string architectureName  = "";

                foreach (string root in windowsSdkDirRoots)
                {
                    string[] windowsHeaderFiles = Directory.GetFiles(root, "Windows.h", SearchOption.AllDirectories);
                    foreach (string file in windowsHeaderFiles)
                    {
                        Match match = Regex.Match(file, "\\\\Include\\\\(?<version>[0-9\\.]*?)\\\\um");
                        if (match.Success && match.Groups["version"].Success)
                        {
                            windowsSdkDirRoot = root;
                            windowsSdkVersion = match.Groups["version"].Value;
                        }
                    }
                    if (!string.IsNullOrEmpty(windowsSdkDirRoot)) { break; }
                }

                // アーキテクチャー名取得
                string? processorArchitecture = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
                if (processorArchitecture is null)
                {
                    // 取得できない場合は x86
                    architectureName = "x86";
                }
                else if (processorArchitecture.Equals("IA64") || processorArchitecture.Equals("AMD64"))
                {
                    // 64ビット
                    architectureName = "x64";
                }
                else
                {
                    // それ以外はそのまま適用
                    architectureName = processorArchitecture;
                }

                // ディレクトリ決定
                windowsBinDir = $"{windowsSdkDirRoot}\\bin\\{windowsSdkVersion}\\{architectureName}";
            }

            // シェーダーコンバート
            {
                // エラーファイル一覧
                List<string> errorFilepaths = new List<string>();

                ReaderWriterLockSlim locker = new ReaderWriterLockSlim();

                // シェーダーコンバート
                Parallel.ForEach(fileData.Targets, target =>
                {
                    // シェーダーコンパイラー起動
                    Process process = new Process();
                    process.StartInfo.FileName = $"{windowsBinDir}\\fxc.exe";
                    process.StartInfo.CreateNoWindow         = true;
                    process.StartInfo.UseShellExecute        = false;
                    process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.RedirectStandardError  = true;
                    process.ErrorDataReceived += ConsoleError_;

                    // 成功フラグ
                    bool is_success = true;

                    if (target.VertexShader is not null)
                    {
                        // 頂点シェーダー
                        string inputPath   = $"{workingPath}\\{target.VertexShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.vs.cso";
                        string profileName = $"vs_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_VERTEX_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.VertexShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.GeometryShader is not null)
                    {
                        // ジオメトリシェーダー
                        string inputPath   = $"{workingPath}\\{target.GeometryShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.gs.cso";
                        string profileName = $"gs_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_GEOMETRY_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.GeometryShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.PixelShader is not null)
                    {
                        // ピクセルシェーダー
                        string inputPath   = $"{workingPath}\\{target.PixelShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.ps.cso";
                        string profileName = $"ps_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_PIXEL_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.PixelShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.ComputeShader is not null)
                    {
                        // コンピュートシェーダー
                        string inputPath   = $"{workingPath}\\{target.ComputeShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.cs.cso";
                        string profileName = $"cs_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_COMPUTE_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.ComputeShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.DomainShader is not null)
                    {
                        // ドメインシェーダー
                        string inputPath   = $"{workingPath}\\{target.DomainShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.ds.cso";
                        string profileName = $"ds_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_DOMAIN_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.DomainShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.HullShader is not null)
                    {
                        // ハルシェーダー
                        string inputPath   = $"{workingPath}\\{target.HullShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.hs.cso";
                        string profileName = $"hs_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_HULL_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.HullShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.AmplificationShader is not null)
                    {
                        // アンプリフィケーションシェーダー
                        string inputPath   = $"{workingPath}\\{target.AmplificationShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.as.cso";
                        string profileName = $"as_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_AMPLIFICATION_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.AmplificationShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }
                    if (target.MeshShader is not null)
                    {
                        // メッシュシェーダー
                        string inputPath   = $"{workingPath}\\{target.MeshShader}";
                        string outputPath  = $"{workingPath}\\{target.Name}.ms.cso";
                        string profileName = $"ms_{fileData.Setting.Profile}";
                        if (Compile_(process, target.Name, inputPath, outputPath, profileName, new List<string> { "BEL_MESH_SHADER=1" }) != 0)
                        {
                            is_success = false;
                            locker.EnterWriteLock();
                            try
                            {
                                errorFilepaths.Add(target.MeshShader);
                            }
                            finally
                            {
                                locker.ExitWriteLock();
                            }
                        }
                    }

                    // 成功したら中間ファイルを出力する
                    if (is_success)
                    {
                        string[] intermediateFiles = Directory.GetFiles(workingPath, $"{target.Name}.*.cso", SearchOption.AllDirectories);
                        foreach (string file in intermediateFiles) { File.Copy(file, $"{outputDir}\\{Path.GetFileName(file)}"); }
                    }
                });

                // 失敗したらここで止める
                if (errorFilepaths.Count != 0)
                {
                    foreach (string filepath in errorFilepaths)
                    {
                        Console.Error.WriteLine($"Compile failed [{filepath}]");
                    }
                    return 3;
                }
            }

            return 0;
        }

        /// <summary>
        /// 標準エラー出力
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        static void ConsoleError_(object sender, DataReceivedEventArgs args)
        {
            Console.Error.WriteLine(args.Data);
        }

        /// <summary>
        /// コンパイル
        /// </summary>
        /// <param name="process"></param>
        /// <param name="name"></param>
        /// <param name="inputPath"></param>
        /// <param name="outputPath"></param>
        /// <param name="profileName"></param>
        /// <param name="defines"></param>
        /// <returns></returns>
        static int Compile_(Process process, string name, string inputPath, string outputPath, string profileName, List<string> defines)
        {
            process.StartInfo.Arguments = $"{inputPath} /T {profileName} /E main /Fo {outputPath}";
            foreach (string define in defines)
            {
                process.StartInfo.Arguments += $" /D {define}";
            }
            Console.WriteLine($"Convert shader... [{name} ({profileName})]");

            process.Start();
            process.BeginOutputReadLine();
            process.BeginErrorReadLine();
            process.WaitForExit();
            process.CancelOutputRead();
            process.CancelErrorRead();

            return process.ExitCode;
        }
    }
}
