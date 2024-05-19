/*!
 * @file   Program.cs
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
using CommandLine;
using System.Text.RegularExpressions;

namespace ShaderArchiver
{
    /// <summary>
    /// オプション
    /// </summary>
    class Options
    {
        [Option('i', "input-dir", Required = true, HelpText = "入力ディレクトリ")]
        public string InputDir { get; set; } = "";
        [Option('n', "name", Required = true, HelpText = "シェーダー名")]
        public string Name { get; set; } = "";
        [Option('o', "output", HelpText = "出力ファイル")]
        public string? Output { get; set; } = null;
    }

    /// <summary>
    /// シェーダーファイルパスリスト
    /// </summary>
    class ShaderFiles
    {
        public string? VertexShaderFilepath { get; set; } = null;
        public string? GeometryShaderFilepath { get; set; } = null;
        public string? PixelShaderFilepath { get; set; } = null;
        public string? ComputeShaderFilepath { get; set; } = null;
        public string? DomainShaderFilepath { get; set; } = null;
        public string? HullShaderFilepath { get; set; } = null;
        public string? AmplificationShaderFilepath { get; set; } = null;
        public string? MeshShaderFilepath { get; set; } = null;
    }

    /// <summary>
    /// シェーダー種別
    /// </summary>
    enum ShaderType
    {
        VertexPixel,          // vs + ps
        VertexGeometryPixel,  // vs + gs + ps
        Tessellation,         // hs + vs + ds + ps
        TessellationGeometry, // hs + vs + ds + gs + ps
        Compute,              // cs
        Mesh,                 // ms + ps
        MeshAmplification,    // as + ms + ps
        Unknown
    }

    /// <summary>
    /// 
    /// </summary>
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

            // 出力先決定
            string output = $"{options.InputDir}/{options.Name}.bshar";
            if (options.Output is not null) { output = options.Output; }

            // 出力先のディレクトリがなければ作る
            string? outputDir = Path.GetDirectoryName(output);
            if (outputDir is null) { return 1; }
            if (!Directory.Exists(outputDir)) { Directory.CreateDirectory(outputDir); }

            // 入力されたディレクトリの中から指定された名前の中間ファイルを取得
            string[] files = Directory.GetFiles(options.InputDir, $"{options.Name}.*.cso", SearchOption.TopDirectoryOnly);
            if (files.Length == 0)
            {
                return 2;
            }

            // シェーダーの組み合わせを決定
            ShaderFiles shaderFiles = new ShaderFiles();
            foreach (string file in files)
            {
                Match match = Regex.Match(file, $".*\\\\{options.Name}\\.(?<suffix>[a-zA-Z]+)\\.cso$");
                if (match.Success && match.Groups["suffix"].Success)
                {
                    string suffix = match.Groups["suffix"].Value;
                    if      (suffix.Equals("vs")) { shaderFiles.VertexShaderFilepath        = file; }
                    else if (suffix.Equals("gs")) { shaderFiles.GeometryShaderFilepath      = file; }
                    else if (suffix.Equals("ps")) { shaderFiles.PixelShaderFilepath         = file; }
                    else if (suffix.Equals("cs")) { shaderFiles.ComputeShaderFilepath       = file; }
                    else if (suffix.Equals("ds")) { shaderFiles.DomainShaderFilepath        = file; }
                    else if (suffix.Equals("hs")) { shaderFiles.HullShaderFilepath          = file; }
                    else if (suffix.Equals("as")) { shaderFiles.AmplificationShaderFilepath = file; }
                    else if (suffix.Equals("ms")) { shaderFiles.MeshShaderFilepath          = file; }
                }
            }

            // 組み合わせ
            ShaderType shaderType = ShaderType.Unknown;

            if (shaderFiles.VertexShaderFilepath is not null && shaderFiles.PixelShaderFilepath is not null)
            {
                if (shaderFiles.GeometryShaderFilepath is not null)
                {
                    if (shaderFiles.HullShaderFilepath is not null && shaderFiles.DomainShaderFilepath is not null)
                    {
                        // hs + vs + ds + gs + ps
                        shaderType = ShaderType.TessellationGeometry;
                    }
                    else
                    {
                        // vs + gs + ps
                        shaderType = ShaderType.VertexGeometryPixel;
                    }
                }
                else
                {
                    if (shaderFiles.HullShaderFilepath is not null && shaderFiles.DomainShaderFilepath is not null)
                    {
                        // hs + vs + ds + ps
                        shaderType = ShaderType.Tessellation;
                    }
                    else
                    {
                        // vs + ps
                        shaderType = ShaderType.VertexPixel;
                    }
                }
            }
            else if (shaderFiles.ComputeShaderFilepath is not null)
            {
                // cs
                shaderType = ShaderType.Compute;
            }
            else if (shaderFiles.MeshShaderFilepath is not null && shaderFiles.PixelShaderFilepath is not null)
            {
                if (shaderFiles.AmplificationShaderFilepath is not null)
                {
                    // as + ms + ps
                    shaderType = ShaderType.MeshAmplification;
                }
                else
                {
                    // ms + ps
                    shaderType = ShaderType.Mesh;
                }
            }
            // Unknown ならここで終了
            if (shaderType == ShaderType.Unknown) { return 3; }

            // アーカイブ
            Console.WriteLine($"Shader archive. [Type={shaderType}, Name={options.Name}]");
            if (!Archive(shaderType, shaderFiles, output))
            {
                return 3;
            }

            return 0;
        }

        /// <summary>
        /// アーカイブ
        /// </summary>
        /// <param name="type"></param>
        /// <param name="shaderFiles"></param>
        /// <param name="output"></param>
        /// <returns></returns>
        static bool Archive(ShaderType type, ShaderFiles shaderFiles, string output)
        {
            // ファイルオープン
            using (FileStream stream = new FileStream(output, FileMode.Create, FileAccess.Write))
            {
                using (BinaryWriter writer = new BinaryWriter(stream))
                {
                    // ヘッダー書き出し
                    {
                        // マジック
                        char[] magic = { 'B', 'S', 'H', 'A' };
                        writer.Write(magic);

                        // シェーダー種別
                        writer.Write((char)type);
                    }
                }
            }

            return true;
        }
    }
}