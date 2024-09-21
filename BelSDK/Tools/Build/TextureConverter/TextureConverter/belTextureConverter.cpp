/*!
 * @file   belTextureConverter.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "image/belImage.h"
#include "belTextureConverter.h"

namespace bel {
//-----------------------------------------------------------------------------
// ctor / dtor
//-----------------------------------------------------------------------------
TextureConverter::TextureConverter() {}
//-----------------------------------------------------------------------------
TextureConverter::~TextureConverter() {}
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
HRESULT TextureConverter::initialize()
{
    // WIC インターフェースを生成
    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory2,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&mpFactory)
    );
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("WICファクトリーの生成に失敗しました\n");
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
        return hr;
    }
    return S_OK;
}
//-----------------------------------------------------------------------------
// read / write
//-----------------------------------------------------------------------------
HRESULT TextureConverter::readFile(Image& image, const std::string& filepath)
{
    // ファイルパスを wchar に変換
    std::unique_ptr<WCHAR[]> filepath_w = std::make_unique<WCHAR[]>(MAX_PATH);
    {
        size_t ret;
        mbstowcs_s(&ret, filepath_w.get(), MAX_PATH, filepath.c_str(), filepath.size());
    }

    // 画像のデコーダーを生成
    Microsoft::WRL::ComPtr<IWICBitmapDecoder> p_decoder;
    HRESULT hr = mpFactory->CreateDecoderFromFilename(
        filepath_w.get(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,
        p_decoder.GetAddressOf()
    );
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("画像の読み込みに失敗しました\n");
        BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
        return hr;
    }

    // 画像データの取得
    // @note 必要になったら複数フレームの取得も実装する
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> p_frame;
    hr = p_decoder->GetFrame(0, p_frame.GetAddressOf());
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("画像の取得に失敗しました\n");
        BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
        return hr;
    }

    // 画像プロパティを取得
    ImageProperty image_property;
    {
        // 画像サイズ
        {
            UINT w, h;
            hr = p_frame->GetSize(&w, &h);
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("サイズの取得に失敗しました\n");
                BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
                return hr;
            }
        }

        // フォーマット取得
        {
            WICPixelFormatGUID format_guid;
            hr = p_frame->GetPixelFormat(&format_guid);
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("フォーマットの取得に失敗しました\n");
                BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
                return hr;
            }

            // 処理しやすいように一部のフォーマットは変換する
            auto check_format = [&format_guid](const GUID& guid)
            {
                return std::memcmp(&guid, &format_guid, sizeof(WICPixelFormatGUID));
            };

            //
            if (check_format(GUID_WICPixelFormat24bppRGB))
            {
                // アルファ無しはアルファありに統一
                image_property.format = gfx::TextureFormat::cR8G8B8A8_sRGB;
                image_property.convert_format_guid = GUID_WICPixelFormat32bppRGBA;
            }
            else if (check_format(GUID_WICPixelFormat24bppBGR))
            {
                // BGR の並びは RGB に変更
                image_property.format = gfx::TextureFormat::cR8G8B8A8_sRGB;
                image_property.convert_format_guid = GUID_WICPixelFormat32bppRGBA;
            }

            image_property.file_format_guid = format_guid;
        }

        // その他のフォーマットはいったん固定
        image_property.depth      = 1;
        image_property.array_size = 1;
        image_property.mip_levels = 1;
        image_property.dimension  = gfx::TextureDimension::c2D;
    }

    // 画像プロパティを使って画像データを生成
    {
        Image::InitializeArg init_arg;
        init_arg.width      = image_property.width;
        init_arg.height     = image_property.height;
        init_arg.depth      = image_property.depth;
        init_arg.mip_levels = image_property.mip_levels;
        init_arg.format     = image_property.format;
        switch (image_property.dimension)
        {
        case gfx::TextureDimension::c1DArray:
        case gfx::TextureDimension::c2DArray:
        case gfx::TextureDimension::cCubeArray:
            init_arg.depth = image_property.array_size;
            break;

        case gfx::TextureDimension::c3D:
            init_arg.depth = image_property.depth;
            break;

        default:
            init_arg.depth = 1;
        }

        if (!image.initialize(init_arg))
        {
            hr = E_FAIL;
            BEL_ERROR_LOG("フォーマットの取得に失敗しました\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr));
            return hr;
        }
    }

    return S_OK;
}
//-----------------------------------------------------------------------------
}