/*!
 * @file   main.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <algorithm>
// windows
#include <wincodec.h>
#include <wrl/client.h>
// bel
#include "common/belOptionParser.h"
#include "graphics/common/belGraphicsTextureType.h"
#include "image/belImage.h"
#include "image/belImageConverter.h"
#include "resource/belResourceTexture.h"

#include "belTextureConverter.h"

namespace
{

//! 画像プロパティ
struct TextureProperty
{
    size_t width        = 0;
    size_t height       = 0;
    size_t depth        = 0;
    size_t array_size   = 0;
    size_t mip_levels   = 0;
    bel::gfx::TextureFormat    format    = bel::gfx::TextureFormat::cUnknown;
    bel::gfx::TextureDimension dimension = bel::gfx::TextureDimension::c2D;
    WICPixelFormatGUID file_format  = GUID_NULL;
    GUID               convert_guid = GUID_NULL;
};

}

int belMain(int argc, const char** argv)
{
    // オプション解析
    std::string input_filepath;
    std::string output_filepath;
    std::string format_name;

    {
        bel::OptionParser parser;
        parser.parse(argc, argv);

        // ヘルプが立ってる場合は出力
        if (parser.hasHelp())
        {
            BEL_INFO_LOG("--input  / -i    : 入力ファイルパス\n");
            BEL_INFO_LOG("--output / -o    : 出力ファイルパス\n");
            BEL_INFO_LOG("--format / -f    : 変換フォーマット\n");
            BEL_INFO_LOG("format list:\n");
            BEL_INFO_LOG("<< 8bit >>\n");
            BEL_INFO_LOG("\tr8_unorm, r8_snorm, a8_unorm\n");
            BEL_INFO_LOG("<< 16bit >>\n");
            BEL_INFO_LOG("\tr8g8_unorm, r8g8_snorm\n");
            BEL_INFO_LOG("<< 32bit >>\n");
            BEL_INFO_LOG("\tr8g8b8a8_unorm, r8g8b8a8_snorm, r8g8b8a8_srgb");
            return 0;
        }

        for (uint32_t i_option = 0; i_option < parser.getNumOption(); ++i_option)
        {
            const bel::OptionParser::OptionValue& option = parser.getOption(i_option);
            if (std::strcmp(option.first.c_str(), "--input") == 0 ||
                std::strcmp(option.first.c_str(), "-i") == 0)
            {
                input_filepath = option.second;
            }
            else if (std::strcmp(option.first.c_str(), "--output") == 0 ||
                     std::strcmp(option.first.c_str(), "-o") == 0)
            {
                output_filepath = option.second;
            }
            else if (std::strcmp(option.first.c_str(), "--format") == 0 ||
                     std::strcmp(option.first.c_str(), "-f") == 0)
            {
                format_name = option.second;
                std::transform(format_name.begin(), format_name.end(), format_name.begin(), std::tolower);
            }
        }
    }

    // 必須オプションの指定が無ければ終了
    if (input_filepath.empty() || output_filepath.empty() || format_name.empty())
    {
        BEL_ERROR_LOG("必須オプションが指定されていません\n");
        return -1;
    }

    // @TODO: 出力ファイルの指定が無ければ入力ファイル名を使う

    // 出力フォーマット
    bel::gfx::TextureFormat output_format = bel::gfx::TextureFormat::cUnknown;
    if      (std::strcmp(format_name.c_str(), "r8_unorm")       == 0) { output_format = bel::gfx::TextureFormat::cR8_uNorm; }
    else if (std::strcmp(format_name.c_str(), "r8_snorm")       == 0) { output_format = bel::gfx::TextureFormat::cR8_sNorm; }
    else if (std::strcmp(format_name.c_str(), "a8_unorm")       == 0) { output_format = bel::gfx::TextureFormat::cA8_uNorm; }
    else if (std::strcmp(format_name.c_str(), "r8g8_unorm")     == 0) { output_format = bel::gfx::TextureFormat::cR8G8_uNorm; }
    else if (std::strcmp(format_name.c_str(), "r8g8_snorm")     == 0) { output_format = bel::gfx::TextureFormat::cR8G8_sNorm; }
    else if (std::strcmp(format_name.c_str(), "r8g8b8a8_unorm") == 0) { output_format = bel::gfx::TextureFormat::cR8G8B8A8_uNorm; }
    else if (std::strcmp(format_name.c_str(), "r8g8b8a8_snorm") == 0) { output_format = bel::gfx::TextureFormat::cR8G8B8A8_sNorm; }
    else if (std::strcmp(format_name.c_str(), "r8g8b8a8_srgb")  == 0) { output_format = bel::gfx::TextureFormat::cR8G8B8A8_sRGB; }
    // 
    if (output_format == bel::gfx::TextureFormat::cUnknown)
    {
        BEL_ERROR_LOG("非対応の変換フォーマットが指定されました\n");
        return -1;
    }

    // ファイル名を WCHAR に変換
    std::unique_ptr<WCHAR[]> input_filepath_w = std::make_unique<WCHAR[]>(MAX_PATH);
    std::unique_ptr<WCHAR[]> output_filepath_w = std::make_unique<WCHAR[]>(MAX_PATH);
    {
        size_t ret;
        mbstowcs_s(&ret, input_filepath_w.get(), MAX_PATH, input_filepath.c_str(), input_filepath.size());
        mbstowcs_s(&ret, output_filepath_w.get(), MAX_PATH, output_filepath.c_str(), output_filepath.size());
    }

    // COMコンポーネントの初期化
    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("COMの初期化に失敗しました\n");
        return hr;
    }

    // クラス生成
    {
        bel::TextureConverter converter;
        hr = converter.initialize();
        if (FAILED(hr)) { return hr; }

        // 画像ファイルの読み込み
        bel::Image image;
        hr = converter.readFile(image, input_filepath);
        if (FAILED(hr)) { return hr; }

        // フォーマット変換
        bel::Image output_image = bel::img::Converter::ConvertFormat(image, output_format);
        if (output_image.getFormat() == bel::gfx::TextureFormat::cUnknown)
        {
            BEL_ERROR_LOG("フォーマットの変換に失敗しました\n");
            return -2;
        }

        // ミップマップを生成する


        // 出力
        {
            bel::res::Texture texture = bel::res::Texture::Create(output_image);
            if (!texture.isValid())
            {
                BEL_ERROR_LOG("リソースの生成に失敗しました\n");
                return -2;
            }

            // 書き出し
            if (!bel::res::Loader::GetInstance().writeSync(output_filepath, texture))
            {
                BEL_ERROR_LOG("出力に失敗しました\n");
                return -3;
            }
        }
    }

    // 終了処理
    CoUninitialize();

    return 0;
}
