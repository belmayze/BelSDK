/*!
 * @file   main.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// windows
#include <wincodec.h>
#include <wrl/client.h>
//
#include "belOptionParser.h"

namespace
{

//-----------------------------------------------------------------------------
// テクスチャーフォーマット
//-----------------------------------------------------------------------------
enum class TextureFormat
{
    // Unknown
    cUnknown,

    // 8bit カラー
    cR8_uNorm,           //!< 1ch 8ビット符号なし正規化整数
    cR8_sNorm,           //!< 1ch 8ビット符号あり正規化整数
    cR8_uInt,            //!< 1ch 8ビット符号なし整数
    cR8_sInt,            //!< 1ch 8ビット符号あり整数
    cA8_uNorm,           //!< 1ch 32ビット符号なし正規化整数
    cR8G8_uNorm,         //!< 2ch 16ビット符号なし正規化整数
    cR8G8_sNorm,         //!< 2ch 16ビット符号あり正規化整数
    cR8G8_uInt,          //!< 2ch 16ビット符号なし整数
    cR8G8_sInt,          //!< 2ch 16ビット符号あり整数
    cR8G8B8A8_uNorm,     //!< 4ch 32ビット符号なし正規化整数
    cR8G8B8A8_sNorm,     //!< 4ch 32ビット符号あり正規化整数
    cR8G8B8A8_sRGB,      //!< 4ch 32ビット符号なし正規化整数（sRGB）
    cR8G8B8A8_uInt,      //!< 4ch 32ビット符号なし整数
    cR8G8B8A8_sInt,      //!< 4ch 32ビット符号あり整数

    // 16ビットカラー
    cR16_uNorm,          //!< 1ch 16ビット符号なし正規化整数
    cR16_sNorm,          //!< 1ch 16ビット符号あり正規化整数
    cR16_uInt,           //!< 1ch 16ビット符号なし整数
    cR16_sInt,           //!< 1ch 16ビット符号あり整数
    cR16_Float,          //!< 1ch 16ビット符号あり浮動小数
    cR16G16_uNorm,       //!< 2ch 32ビット符号なし正規化整数
    cR16G16_sNorm,       //!< 2ch 32ビット符号あり正規化整数
    cR16G16_uInt,        //!< 2ch 32ビット符号なし整数
    cR16G16_sInt,        //!< 2ch 32ビット符号あり整数
    cR16G16_Float,       //!< 2ch 32ビット符号あり浮動小数
    cR16G16B16A16_uNorm, //!< 4ch 64ビット符号なし正規化整数
    cR16G16B16A16_sNorm, //!< 4ch 64ビット符号あり正規化整数
    cR16G16B16A16_uInt,  //!< 4ch 64ビット符号なし整数
    cR16G16B16A16_sInt,  //!< 4ch 64ビット符号あり整数
    cR16G16B16A16_Float, //!< 4ch 64ビット符号あり浮動小数

    // 32ビットカラー
    cR32_uInt,           //!< 1ch 32ビット符号なし整数
    cR32_sInt,           //!< 1ch 32ビット符号あり整数
    cR32_Float,          //!< 1ch 32ビット符号あり浮動小数
    cR32G32_uInt,        //!< 2ch 64ビット符号なし整数
    cR32G32_sInt,        //!< 2ch 64ビット符号あり整数
    cR32G32_Float,       //!< 2ch 64ビット符号あり浮動小数
    cR32G32B32_uInt,     //!< 3ch 96ビット符号なし整数
    cR32G32B32_sInt,     //!< 3ch 96ビット符号あり整数
    cR32G32B32_Float,    //!< 3ch 96ビット符号あり浮動小数
    cR32G32B32A32_uInt,  //!< 4ch 128ビット符号なし整数
    cR32G32B32A32_sInt,  //!< 4ch 128ビット符号あり整数
    cR32G32B32A32_Float, //!< 4ch 128ビット符号あり浮動小数

    // その他カラー
    cR5G6B5_uNorm,       //!< 3ch 32ビット符号なし正規化整数
    cR5G5B5A1_uNorm,     //!< 4ch 32ビット符号なし正規化整数
    cR10G10B10A2_uNorm,  //!< 4ch 32ビット符号なし正規化整数
    cR10G10B10A2_uInt,   //!< 4ch 32ビット符号なし整数
    cR11G11B10_uFloat,   //!< 3ch 32ビット符号なし浮動小数

    // デプス
    cD16_uNorm,          //!< 16ビット正規化整数デプス
    cD24_uNorm_S8_uInt,  //!< 24ビット正規化整数デプス + 8ビット整数ステンシル
    cD32_Float,          //!< 32ビット浮動小数デプス
    cD32_Float_S8_uInt,  //!< 32ビット浮動小数デプス + 8ビット整数ステンシル

    // BC圧縮
    cBC1_uNorm,          //!< 4bpp 3ch もしくは 4ch（2値アルファ）符号なし正規化整数
    cBC1_sRGB,           //!< 4bpp 3ch もしくは 4ch（2値アルファ）符号なし正規化整数（sRGB）
    cBC2_uNorm,          //!< 8bpp 4ch 符号なし16階調正規化整数
    cBC2_sRGB,           //!< 8bpp 4ch 符号なし16階調正規化整数（sRGB）
    cBC3_uNorm,          //!< 8bpp 4ch 符号なし正規化整数
    cBC3_sRGB,           //!< 8bpp 4ch 符号なし正規化整数（sRGB）
    cBC4_uNorm,          //!< 4bpp 1ch 符号なし正規化整数
    cBC4_sNorm,          //!< 4bpp 1ch 符号あり正規化整数
    cBC5_uNorm,          //!< 8bpp 2ch 符号なし正規化整数
    cBC5_sNorm,          //!< 8bpp 2ch 符号あり正規化整数
    cBC6_uFloat,         //!< 8bpp 3ch 符号なし浮動小数（FP16）
    cBC6_sFloat,         //!< 8bpp 3ch 符号あり浮動小数（FP16）
    cBC7_uNorm,          //!< 8bpp 3ch もしくは 4ch 符号なし正規化整数
    cBC7_sRGB,           //!< 8bpp 3ch もしくは 4ch 符号なし正規化整数（sRGB）

    cNum
};

//! 画像プロパティ
struct TextureProperty
{
    size_t        width        = 0;
    size_t        height       = 0;
    size_t        depth        = 0;
    size_t        array_size   = 0;
    size_t        mip_levels   = 0;
    TextureFormat format       = TextureFormat::cUnknown;
    GUID          convert_guid = GUID_NULL;
};

}

int main(int argc, const char* argv[])
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
            }
        }
    }

    // 必須オプションの指定が無ければ終了
    if (input_filepath.empty() || output_filepath.empty() || format_name.empty())
    {
        printf("必須オプションが指定されていません\n");
        return 1;
    }

    // @TODO: 出力ファイルの指定が無ければ入力ファイル名を使う

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
        printf("COMの初期化に失敗しました\n");
        return 2;
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
            printf("WICファクトリーの生成に失敗しました\n");
            return 2;
        }

        // WIC 初期化
        Microsoft::WRL::ComPtr<IWICBitmapDecoder> p_decoder;
        hr = p_factory->CreateDecoderFromFilename(input_filepath_w.get(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, p_decoder.GetAddressOf());
        if (FAILED(hr))
        {
            printf("デコーダーの生成に失敗しました\n");
            return 2;
        }

        // フレーム取得
        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> p_frame;
        hr = p_decoder->GetFrame(0, p_frame.GetAddressOf());
        if (FAILED(hr))
        {
            printf("画像の取得に失敗しました\n");
            return 2;
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
                    printf("プロパティのアクセスに失敗しました\n");
                    return 2;
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
                    printf("プロパティのアクセスに失敗しました\n");
                    return 2;
                }

                auto checker = [&format, &property](const GUID& guid, TextureFormat texture_format, GUID convert_guid)
                {
                    if (std::memcmp(&guid, &format, sizeof(WICPixelFormatGUID)) == 0)
                    {
                        property.format       = texture_format;
                        property.convert_guid = convert_guid;
                        return true;
                    }
                    return false;
                };

                if      (checker(GUID_WICPixelFormat24bppRGB, TextureFormat::cR8G8B8A8_uNorm, GUID_WICPixelFormat32bppRGBA)) {}
                else if (checker(GUID_WICPixelFormat24bppBGR, TextureFormat::cR8G8B8A8_uNorm, GUID_WICPixelFormat32bppRGBA)) {}
            }
        }
    }

    // 終了処理
    CoUninitialize();

    return 0;
}
