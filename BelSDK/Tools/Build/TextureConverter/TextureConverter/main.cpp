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
        BEL_ERROR("必須オプションが指定されていません\n");
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
        BEL_ERROR("非対応の変換フォーマットが指定されました\n");
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
        BEL_ERROR("COMの初期化に失敗しました\n");
        return hr;
    }

    // WIC Interface
    {
        Microsoft::WRL::ComPtr<IWICImagingFactory2> p_factory;
        hr = CoCreateInstance(
            CLSID_WICImagingFactory2,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&p_factory)
        );
        if (FAILED(hr))
        {
            BEL_ERROR("WICファクトリーの生成に失敗しました\n");
            return hr;
        }

        // WIC 初期化
        Microsoft::WRL::ComPtr<IWICBitmapDecoder> p_decoder;
        hr = p_factory->CreateDecoderFromFilename(input_filepath_w.get(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, p_decoder.GetAddressOf());
        if (FAILED(hr))
        {
            BEL_ERROR("画像の読み込みに失敗しました\n");
            return hr;
        }

        // フレーム取得
        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> p_frame;
        hr = p_decoder->GetFrame(0, p_frame.GetAddressOf());
        if (FAILED(hr))
        {
            BEL_ERROR("画像の取得に失敗しました\n");
            return hr;
        }

        // プロパティ取得
        TextureProperty property;
        {
            // 画像サイズ
            {
                UINT w, h;
                hr = p_frame->GetSize(&w, &h);
                if (FAILED(hr))
                {
                    BEL_ERROR("プロパティのアクセスに失敗しました\n");
                    return hr;
                }
                property.width = w;
                property.height = h;
            }

            // フォーマット取得
            {
                WICPixelFormatGUID format;
                hr = p_frame->GetPixelFormat(&format);
                if (FAILED(hr))
                {
                    BEL_ERROR("プロパティのアクセスに失敗しました\n");
                    return hr;
                }

                auto checker = [&format, &property](const GUID& guid, bel::gfx::TextureFormat texture_format, GUID convert_guid)
                {
                    if (std::memcmp(&guid, &format, sizeof(WICPixelFormatGUID)) == 0)
                    {
                        property.format       = texture_format;
                        property.convert_guid = convert_guid;
                        return true;
                    }
                    return false;
                };

                // @TODO: 特定のフォーマットのみに限定
                if      (checker(GUID_WICPixelFormat24bppRGB, bel::gfx::TextureFormat::cR8G8B8A8_uNorm, GUID_WICPixelFormat32bppRGBA)) {}
                else if (checker(GUID_WICPixelFormat24bppBGR, bel::gfx::TextureFormat::cR8G8B8A8_uNorm, GUID_WICPixelFormat32bppRGBA)) {}

                // 出力
                property.file_format = format;
            }

            // いったん固定
            property.mip_levels = 1;
            property.array_size = 1;
            property.depth      = 1;
            property.dimension  = bel::gfx::TextureDimension::c2D;
        }

        // 画像データを生成
        bel::Image image;
        {
            bel::Image::InitializeArg init_arg;
            init_arg.width      = property.width;
            init_arg.height     = property.height;
            init_arg.array_size = property.array_size;
            init_arg.mip_levels = property.mip_levels;
            init_arg.format     = property.format;

            // @TODO: いったん DIMENSION_2D のみ
            if (!image.initialize2D(init_arg))
            {
                BEL_ERROR("画像の初期化に失敗しました\n");
                return hr;
            }

            // @TODO: いったん１枚の画像のみ
            // 画像プロパティ取得
            const bel::Image::ImageProperty& image_property = image.getImageProperty(0);

            // GUID が NULL であれば変換不要
            if (std::memcmp(&property.convert_guid, &GUID_NULL, sizeof(GUID)) == 0)
            {
                hr = p_frame->CopyPixels(
                    nullptr,
                    static_cast<UINT>(image_property.row_pitch),
                    static_cast<UINT>(image_property.slice_pitch),
                    image.getMemoryPtr(image_property.memory_offset)
                );
                if (FAILED(hr))
                {
                    BEL_ERROR("画像のコピーに失敗しました\n");
                    return hr;
                }
            }
            else
            {
                // 指定したフォーマットにコンバートする
                Microsoft::WRL::ComPtr<IWICFormatConverter> p_converter;
                hr = p_factory->CreateFormatConverter(p_converter.GetAddressOf());
                if (FAILED(hr))
                {
                    BEL_ERROR("画像コンバーターの生成に失敗しました\n");
                    return hr;
                }

                BOOL can_convert = FALSE;
                hr = p_converter->CanConvert(property.file_format, property.convert_guid, &can_convert);
                if (FAILED(hr))
                {
                    BEL_ERROR("コンバートチェックに失敗しました\n");
                    return hr;
                }

                // コンバート
                hr = p_converter->Initialize(p_frame.Get(), property.convert_guid, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut);
                if (FAILED(hr))
                {
                    BEL_ERROR("コンバートに失敗しました\n");
                    return hr;
                }

                hr = p_converter->CopyPixels(
                    nullptr,
                    static_cast<UINT>(image_property.row_pitch),
                    static_cast<UINT>(image_property.slice_pitch),
                    image.getMemoryPtr(image_property.memory_offset)
                );
                if (FAILED(hr))
                {
                    BEL_ERROR("画像のコピーに失敗しました\n");
                    return hr;
                }
            }
        }

        // フォーマット変換
        bel::Image output_image = bel::img::Converter::ConvertFormat(image, output_format);
        if (output_image.getFormat() == bel::gfx::TextureFormat::cUnknown)
        {
            BEL_ERROR("フォーマットの変換に失敗しました\n");
            return -2;
        }

        // 出力

    }

    // 終了処理
    CoUninitialize();

    return 0;
}
