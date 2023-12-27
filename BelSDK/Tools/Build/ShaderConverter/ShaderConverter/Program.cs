using CommandLine;
using System.Diagnostics;
using System.Text.RegularExpressions;
using System.Xml.Serialization;

namespace ShaderConverter
{
    class Program
    {
        class Options
        {
            [Option('i', "input", Required = true, HelpText = "入力")]
            public string Input { get; set; } = "";
            [Option('o', "output", HelpText = "出力")]
            public string Output { get; set; } = "";
        }

        static int Main(string[] args)
        {
            ParserResult<Options> parserResult = Parser.Default.ParseArguments<Options>(args);
            if (parserResult.Tag == ParserResultType.NotParsed)
            {
                return 2;
            }
            Options options = parserResult.Value;

            // 入力された xml ファイルを読み込む
            XmlData.ShaderContainer? shaderContainer = null;
            try
            {
                FileStream stream = new FileStream(options.Input, FileMode.Open);

                try
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(XmlData.ShaderContainer));
                    shaderContainer = serializer.Deserialize(stream) as XmlData.ShaderContainer;
                }
                catch (Exception e)
                {
                    Console.Error.WriteLine($"Error: {e.Message}");
                    return 1;
                }
            }
            catch(Exception e)
            {
                Console.Error.WriteLine($"Error: {e.Message}");
                return 1;
            }

            // include に含まれているファイルをすべてコピーする
            string workingPath = $"{Environment.GetEnvironmentVariable("TEMP")}\\BelSDK\\ShaderConverter";
            if (!Directory.Exists(workingPath))
            {
                Directory.CreateDirectory(workingPath);
            }
            else
            {
                string[] files = Directory.GetFiles(workingPath, "*", SearchOption.AllDirectories);
                foreach (string file in files)
                {
                    File.Delete(file);
                }
            }

            string? rootPath = Path.GetDirectoryName(Path.GetFullPath(options.Input));
            if (rootPath is null) { return 1; }

            foreach (XmlData.Include include in shaderContainer.Settings.Includes)
            {
                string path = include.Path;
                if (!Path.IsPathRooted(path)) { path = $"{rootPath}\\{path}"; }

                // ファイルをコピーする
                Console.WriteLine($"Copy from \"{path}\" to working directory");
                string[] files = Directory.GetFiles(path, "*", SearchOption.TopDirectoryOnly);
                foreach (string file in files)
                {
                    File.Copy(file, $"{workingPath}\\{Path.GetFileName(file)}");
                }
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
                            break;
                        }
                    }
                    if (!string.IsNullOrEmpty(windowsSdkDirRoot)) { break; }
                }

                // アーキテクチャー名取得
                string? processorArchitecture = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
                if (processorArchitecture is null) { architectureName = "x86"; }
                else if (processorArchitecture.Equals("IA64") || processorArchitecture.Equals("AMD64")) { architectureName = "x64"; }
                else { architectureName = processorArchitecture; }
                windowsBinDir = $"{windowsSdkDirRoot}\\bin\\{windowsSdkVersion}\\{architectureName}";
            }

            // シェーダーをコンパイルする
            {
                // エラーファイル一覧
                List<string> errorFilepaths = new List<string>();

                // シェーダーコンバート
                foreach (XmlData.Program program in shaderContainer.Programs)
                {
                    Process process = new Process();
                    process.StartInfo.FileName = $"{windowsBinDir}\\fxc.exe";
                    process.StartInfo.CreateNoWindow = true;
                    process.StartInfo.UseShellExecute = false;
                    process.StartInfo.RedirectStandardOutput = true;
                    process.StartInfo.RedirectStandardError = true;
                    process.ErrorDataReceived += ConsoleError_;

                    if (program.VertexShader is not null)
                    {
                        // バーテックスシェーダー
                        string inputPath = $"{workingPath}\\{program.VertexShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.VertexShader.SourceFilePath}.vs.cso";
                        string profileName = $"vs_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_VERTEX_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.GeometryShader is not null)
                    {
                        // ジオメトリシェーダー
                        string inputPath = $"{workingPath}\\{program.GeometryShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.GeometryShader.SourceFilePath}.gs.cso";
                        string profileName = $"gs_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_GEOMETRY_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.PixelShader is not null)
                    {
                        // ピクセルシェーダー
                        string inputPath = $"{workingPath}\\{program.PixelShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.PixelShader.SourceFilePath}.ps.cso";
                        string profileName = $"ps_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_PIXEL_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.ComputeShader is not null)
                    {
                        // コンピュートシェーダー
                        string inputPath = $"{workingPath}\\{program.ComputeShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.ComputeShader.SourceFilePath}.cs.cso";
                        string profileName = $"cs_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_COMPUTE_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.DomainShader is not null)
                    {
                        // ドメインシェーダー
                        string inputPath = $"{workingPath}\\{program.DomainShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.DomainShader.SourceFilePath}.ds.cso";
                        string profileName = $"ds_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_DOMAIN_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.HullShader is not null)
                    {
                        // ハルシェーダー
                        string inputPath = $"{workingPath}\\{program.HullShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.HullShader.SourceFilePath}.hs.cso";
                        string profileName = $"hs_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_HULL_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.AmplificationShader is not null)
                    {
                        // アンプリフィケーションシェーダー
                        string inputPath = $"{workingPath}\\{program.AmplificationShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.AmplificationShader.SourceFilePath}.as.cso";
                        string profileName = $"as_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_AMPLIFICATION_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                    if (program.MeshShader is not null)
                    {
                        // メッシュシェーダー
                        string inputPath = $"{workingPath}\\{program.MeshShader.SourceFilePath}";
                        string outputPath = $"{workingPath}\\{program.MeshShader.SourceFilePath}.ms.cso";
                        string profileName = $"ms_{shaderContainer.Settings.Compile.Profile}";
                        if (ConvertImpl_(process, inputPath, outputPath, profileName, new List<string>() { "BEL_MESH_SHADER=1" }) != 0)
                        {
                            errorFilepaths.Add(inputPath);
                        }
                    }
                }

                // 失敗したらここで止める
                if (errorFilepaths.Count != 0)
                {
                    foreach (string filepath in errorFilepaths)
                    {
                        Console.Error.WriteLine($"Compile failed [{filepath}]");
                    }
                    return 1;
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
        /// コンバート用 exe 呼び出し
        /// </summary>
        /// <param name="process"></param>
        /// <param name="inputPath"></param>
        /// <param name="outputPath"></param>
        /// <param name="profileName"></param>
        /// <param name="defines"></param>
        /// <returns></returns>
        static int ConvertImpl_(Process process, string inputPath, string outputPath, string profileName, List<string> defines)
        {
            process.StartInfo.Arguments = $"{inputPath} /T {profileName} /E main /Fo {outputPath}";
            foreach (string define in defines)
            {
                process.StartInfo.Arguments += $" /D {define}";
            }
            Console.WriteLine($"Convert shader... [{inputPath}]");

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
