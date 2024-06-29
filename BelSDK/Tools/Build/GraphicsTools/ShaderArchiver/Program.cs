/*!
 * @file   Program.cs
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
using CommandLine;
using System.Runtime.InteropServices;
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
        public string? HullShaderFilepath { get; set; } = null;
        public string? DomainShaderFilepath { get; set; } = null;
        public string? AmplificationShaderFilepath { get; set; } = null;
        public string? MeshShaderFilepath { get; set; } = null;
        public string? LibraryShaderFilepath { get; set; } = null;
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
        Library,              // lib
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
                    if      (suffix.Equals("vs"))  { shaderFiles.VertexShaderFilepath        = file; }
                    else if (suffix.Equals("gs"))  { shaderFiles.GeometryShaderFilepath      = file; }
                    else if (suffix.Equals("ps"))  { shaderFiles.PixelShaderFilepath         = file; }
                    else if (suffix.Equals("cs"))  { shaderFiles.ComputeShaderFilepath       = file; }
                    else if (suffix.Equals("ds"))  { shaderFiles.DomainShaderFilepath        = file; }
                    else if (suffix.Equals("hs"))  { shaderFiles.HullShaderFilepath          = file; }
                    else if (suffix.Equals("as"))  { shaderFiles.AmplificationShaderFilepath = file; }
                    else if (suffix.Equals("ms"))  { shaderFiles.MeshShaderFilepath          = file; }
                    else if (suffix.Equals("lib")) { shaderFiles.LibraryShaderFilepath       = file; }
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
            else if (shaderFiles.LibraryShaderFilepath is not null)
            {
                // lib
                shaderType = ShaderType.Library;
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

        [StructLayout(LayoutKind.Sequential)]
        class ArchiveHeader
        {
            // vs
            public uint VertexSize { get; set; } = 0;
            public uint VertexOffset { get; set; } = 0;

            // gs
            public uint GeometrySize { get; set; } = 0;
            public uint GeometryOffset { get; set; } = 0;

            // ps
            public uint PixelSize { get; set; } = 0;
            public uint PixelOffset { get; set; } = 0;

            // cs
            public uint ComputeSize { get; set; } = 0;
            public uint ComputeOffset { get; set; } = 0;

            // hs
            public uint HullSize { get; set; } = 0;
            public uint HullOffset { get; set; } = 0;

            // ds
            public uint DomainSize { get; set; } = 0;
            public uint DomainOffset { get; set; } = 0;

            // as
            public uint AmplificationSize { get; set; } = 0;
            public uint AmplificationOffset { get; set; } = 0;

            // ms
            public uint MeshSize { get; set; } = 0;
            public uint MeshOffset { get; set; } = 0;

            // lib
            public uint LibrarySize { get; set; } = 0;
            public uint LibraryOffset { get; set; } = 0;

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
                    uint baseOffset = 8 + (uint)Marshal.SizeOf(typeof(ArchiveHeader)); // magic + type + padding

                    // ヘッダー書き出し
                    {
                        // マジック
                        char[] magic = { 'B', 'S', 'H', '_' };
                        writer.Write(magic);

                        // バージョン
                        writer.Write((char)0);

                        // シェーダー種別
                        writer.Write((char)type);

                        // パディング
                        writer.Write(new char[] { '\0', '\0' });
                    }

                    // シェーダー実装
                    {
                        ArchiveHeader header = new ArchiveHeader();

                        switch (type)
                        {
                            case ShaderType.VertexPixel:
                                // ヘッダー構築
                                header.VertexSize   = (uint)new FileInfo(shaderFiles.VertexShaderFilepath!).Length;
                                header.VertexOffset = baseOffset;
                                baseOffset += header.VertexSize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.VertexShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.VertexGeometryPixel:
                                // ヘッダー構築
                                header.VertexSize   = (uint)new FileInfo(shaderFiles.VertexShaderFilepath!).Length;
                                header.VertexOffset = baseOffset;
                                baseOffset += header.VertexSize;

                                header.GeometrySize   = (uint)new FileInfo(shaderFiles.GeometryShaderFilepath!).Length;
                                header.GeometryOffset = baseOffset;
                                baseOffset += header.GeometrySize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.VertexShaderFilepath!);
                                WriteShader(writer, shaderFiles.GeometryShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.Tessellation:
                                // ヘッダー構築
                                header.HullSize   = (uint)new FileInfo(shaderFiles.HullShaderFilepath!).Length;
                                header.HullOffset = baseOffset;
                                baseOffset += header.HullSize;

                                header.VertexSize   = (uint)new FileInfo(shaderFiles.VertexShaderFilepath!).Length;
                                header.VertexOffset = baseOffset;
                                baseOffset += header.VertexSize;

                                header.DomainSize   = (uint)new FileInfo(shaderFiles.DomainShaderFilepath!).Length;
                                header.DomainOffset = baseOffset;
                                baseOffset += header.DomainSize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.HullShaderFilepath!);
                                WriteShader(writer, shaderFiles.VertexShaderFilepath!);
                                WriteShader(writer, shaderFiles.DomainShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.TessellationGeometry:
                                // ヘッダー構築
                                header.HullSize   = (uint)new FileInfo(shaderFiles.HullShaderFilepath!).Length;
                                header.HullOffset = baseOffset;
                                baseOffset += header.HullSize;

                                header.VertexSize   = (uint)new FileInfo(shaderFiles.VertexShaderFilepath!).Length;
                                header.VertexOffset = baseOffset;
                                baseOffset += header.VertexSize;

                                header.DomainSize   = (uint)new FileInfo(shaderFiles.DomainShaderFilepath!).Length;
                                header.DomainOffset = baseOffset;
                                baseOffset += header.DomainSize;

                                header.GeometrySize   = (uint)new FileInfo(shaderFiles.GeometryShaderFilepath!).Length;
                                header.GeometryOffset = baseOffset;
                                baseOffset += header.GeometrySize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.HullShaderFilepath!);
                                WriteShader(writer, shaderFiles.VertexShaderFilepath!);
                                WriteShader(writer, shaderFiles.DomainShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.Compute:
                                // ヘッダー構築
                                header.ComputeSize   = (uint)new FileInfo(shaderFiles.ComputeShaderFilepath!).Length;
                                header.ComputeOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.ComputeShaderFilepath!);
                                break;

                            case ShaderType.Mesh:
                                // ヘッダー構築
                                header.MeshSize = (uint)new FileInfo(shaderFiles.MeshShaderFilepath!).Length;
                                header.MeshOffset = baseOffset;
                                baseOffset += header.MeshSize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.MeshShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.MeshAmplification:
                                // ヘッダー構築
                                header.AmplificationSize   = (uint)new FileInfo(shaderFiles.AmplificationShaderFilepath!).Length;
                                header.AmplificationOffset = baseOffset;
                                baseOffset += header.AmplificationSize;

                                header.MeshSize = (uint)new FileInfo(shaderFiles.MeshShaderFilepath!).Length;
                                header.MeshOffset = baseOffset;
                                baseOffset += header.MeshSize;

                                header.PixelSize   = (uint)new FileInfo(shaderFiles.PixelShaderFilepath!).Length;
                                header.PixelOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.AmplificationShaderFilepath!);
                                WriteShader(writer, shaderFiles.MeshShaderFilepath!);
                                WriteShader(writer, shaderFiles.PixelShaderFilepath!);
                                break;

                            case ShaderType.Library:
                                // ヘッダー構築
                                header.LibrarySize   = (uint)new FileInfo(shaderFiles.LibraryShaderFilepath!).Length;
                                header.LibraryOffset = baseOffset;

                                // ヘッダー出力
                                WriteHeader(writer, header);

                                // ファイル出力
                                WriteShader(writer, shaderFiles.LibraryShaderFilepath!);
                                break;
                        }
                    }
                }
            }

            return true;
        }

        /// <summary>
        /// ヘッダー出力
        /// </summary>
        /// <param name="writer"></param>
        /// <param name="header"></param>
        static void WriteHeader(BinaryWriter writer, ArchiveHeader header)
        {
            int    size   = Marshal.SizeOf(typeof(ArchiveHeader));
            byte[] buffer = new byte[size];
            IntPtr ptr    = IntPtr.Zero;

            try
            {
                ptr = Marshal.AllocHGlobal(size);
                Marshal.StructureToPtr(header, ptr, false);
                Marshal.Copy(ptr, buffer, 0, size);
            }
            finally
            {
                if (ptr != IntPtr.Zero) { Marshal.FreeHGlobal(ptr); }
            }
            writer.Write(buffer);
        }

        /// <summary>
        /// シェーダーを書き出す
        /// </summary>
        /// <param name="writer"></param>
        /// <param name="filepath"></param>
        static void WriteShader(BinaryWriter writer, string filepath)
        {
            byte[] fileBytes = File.ReadAllBytes(filepath);
            writer.Write(fileBytes);
        }
    }
}