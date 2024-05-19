/*!
 * @file   Program.cs
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
using CommandLine;

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

            // 

            return 0;
        }
    }
}