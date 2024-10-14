/*!
 * @file   belTextureConverter.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsTextureFormatInfo.h"
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
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
        return hr;
    }
    return S_OK;
}
//-----------------------------------------------------------------------------
// read / write
//-----------------------------------------------------------------------------
HRESULT TextureConverter::readFile(Image& image, const std::string& filepath, size_t mip_levels)
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
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
        return hr;
    }

    // 画像データの取得
    // @todo 必要になったら複数フレームの取得も実装する
    Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> p_frame;
    hr = p_decoder->GetFrame(0, p_frame.GetAddressOf());
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("画像の取得に失敗しました\n");
        BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
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
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            image_property.width  = w;
            image_property.height = h;
        }

        // フォーマット取得
        {
            WICPixelFormatGUID format_guid;
            hr = p_frame->GetPixelFormat(&format_guid);
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("フォーマットの取得に失敗しました\n");
                BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            // 処理しやすいように一部のフォーマットは変換する
            auto check_format = [&format_guid](const GUID& guid)
            {
                return std::memcmp(&guid, &format_guid, sizeof(WICPixelFormatGUID));
            };

            //
            if (check_format(GUID_WICPixelFormat24bppRGB) || check_format(GUID_WICPixelFormat32bppRGB))
            {
                // アルファ無しはアルファありに統一
                image_property.format = gfx::TextureFormat::cR8G8B8A8_sRGB;
                image_property.convert_format_guid = GUID_WICPixelFormat32bppRGBA;
            }
            else if (check_format(GUID_WICPixelFormat24bppBGR) || check_format(GUID_WICPixelFormat32bppBGR))
            {
                // BGR の並びは RGB に変更
                image_property.format = gfx::TextureFormat::cR8G8B8A8_sRGB;
                image_property.convert_format_guid = GUID_WICPixelFormat32bppRGBA;
            }
            else if (check_format(GUID_WICPixelFormat32bppRGBA))
            {
                image_property.format = gfx::TextureFormat::cR8G8B8A8_sRGB;
            }
            else
            {
                hr = E_INVALIDARG;
                BEL_ERROR_LOG("読み込みに対応していないフォーマットです\n");
                BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            image_property.file_format_guid = format_guid;
        }

        // その他のフォーマットはいったん固定
        image_property.depth      = 1;
        image_property.array_size = 1;
        image_property.mip_levels = Math::Min<size_t>(mip_levels, Image::CalcMaximumMipLevels(image_property.width, image_property.height));
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
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }
    }

    // 画像データをコピーする
    if (std::memcmp(&image_property.convert_format_guid, &GUID_NULL, sizeof(GUID)) == 0)
    {
        // @todo 必要になったらミップマップの読み込みも実装する
        const Image::ImageProperty& mip_prop = image.getImageProperty(0);

        // 変換が不要ならそのままコピーする
        hr = p_frame->CopyPixels(
            nullptr,
            static_cast<UINT>(mip_prop.row_pitch),
            static_cast<UINT>(mip_prop.slice_pitch),
            image.getMemoryPtr(mip_prop.memory_offset)
        );
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("ピクセルデータの取得に失敗しました\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }
    }
    else
    {
        // @todo 必要になったらミップマップの読み込みも実装する
        const Image::ImageProperty& mip_prop = image.getImageProperty(0);

        // 指定したフォーマットにコンバートする
        Microsoft::WRL::ComPtr<IWICFormatConverter> p_converter;
        hr = mpFactory->CreateFormatConverter(p_converter.GetAddressOf());
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("コンバーターの生成に失敗しました\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        BOOL can_convert = FALSE;
        hr = p_converter->CanConvert(
            image_property.file_format_guid,
            image_property.convert_format_guid,
            &can_convert
        );
        if (FAILED(hr) || !can_convert)
        {
            BEL_ERROR_LOG("フォーマットがコンバートできない組み合わせです\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        // コンバート
        hr = p_converter->Initialize(
            p_frame.Get(),
            image_property.convert_format_guid,
            WICBitmapDitherTypeNone,
            nullptr, 0,
            WICBitmapPaletteTypeMedianCut
        );
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("コンバートに失敗しました\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        hr = p_converter->CopyPixels(
            nullptr,
            static_cast<UINT>(mip_prop.row_pitch),
            static_cast<UINT>(mip_prop.slice_pitch),
            image.getMemoryPtr(mip_prop.memory_offset)
        );
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("ピクセルデータの取得に失敗しました\n");
            BEL_ERROR_LOG("    対象: %s\n", filepath.c_str());
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }
    }

    return S_OK;
}
//-----------------------------------------------------------------------------
// convert
//-----------------------------------------------------------------------------
HRESULT TextureConverter::convertFormat(Image& output, const Image& input, gfx::TextureFormat format)
{
    // 同じフォーマット
    bool is_same = (input.getFormat() == format);

    // sRGB 違いならそのまま返す
    switch (input.getFormat())
    {
    case gfx::TextureFormat::cR8G8B8A8_sRGB:
    case gfx::TextureFormat::cR8G8B8A8_uNorm:
        is_same = is_same || format == gfx::TextureFormat::cR8G8B8A8_sRGB || format == gfx::TextureFormat::cR8G8B8A8_uNorm;
        break;
    }

    // 同じフォーマットだった場合はコピーして返す
    if (is_same)
    {
        // 画像生成
        Image::InitializeArg init_arg;
        init_arg.width      = input.getWidth();
        init_arg.height     = input.getHeight();
        init_arg.mip_levels = input.getMipLevels();
        init_arg.depth      = input.getDepth();
        init_arg.format     = format;
        init_arg.component_mapping = input.getComponentMapping();
        if (!output.initialize(init_arg))
        {
            BEL_ERROR_LOG("画像ファイルの生成に失敗しました\n");
            return E_FAIL;
        }

        // コピー
        std::memcpy(output.getMemoryPtr(0), input.getMemoryPtr(0), input.getMemorySize());

        return S_OK;
    }

    // 異なるフォーマットの場合は変換します
    {
        Image::InitializeArg init_arg;
        init_arg.width      = input.getWidth();
        init_arg.height     = input.getHeight();
        init_arg.mip_levels = input.getMipLevels();
        init_arg.depth      = input.getDepth();
        init_arg.format     = format;
        init_arg.component_mapping = gfx::TextureFormatInfo::DefaultComponentMapping(format);
        if (!output.initialize(init_arg))
        {
            BEL_ERROR_LOG("画像ファイルの生成に失敗しました\n");
            return E_FAIL;
        }

        // RGBA
        if (input.getFormat() == gfx::TextureFormat::cR8G8B8A8_uNorm ||
            input.getFormat() == gfx::TextureFormat::cR8G8B8A8_sRGB)
        {
            if (format == gfx::TextureFormat::cR8_uNorm)
            {
                // RGBA -> R
                for (size_t i_depth = 0; i_depth < input.getDepth(); ++i_depth)
                {
                    for (size_t i_mip = 0; i_mip < input.getMipLevels(); ++i_mip)
                    {
                        // ミップマップのメモリー取得
                        const Image::ImageProperty& mip_property = input.getImageProperty(i_mip);
                        const uint8_t* src_ptr = input.getMemoryPtr(mip_property.memory_offset);
                        uint8_t*       dst_ptr = output.getMemoryPtr(output.getImageProperty(i_mip).memory_offset);

                        // 変換
                        for (size_t i_height = 0; i_height < mip_property.height; ++i_height)
                        {
                            for (size_t i_width = 0; i_width < mip_property.width; ++i_width)
                            {
                                *dst_ptr = *src_ptr;
                                dst_ptr += 1;
                                src_ptr += 4;
                            }
                        }
                    }
                }
                return S_OK;
            }
        }
    }

    BEL_ERROR_LOG("変換に対応していないフォーマットです\n");
    return E_INVALIDARG;
}
//-----------------------------------------------------------------------------
HRESULT TextureConverter::generateMipLevels(Image& image, uint32_t mip_levels)
{
    // 画像フォーマットから GUID を作る
    WICPixelFormatGUID format_guid;
    if (!FormatToGuid_(format_guid, image.getFormat()))
    {
        BEL_ERROR_LOG("フォーマットから GUID に変換できませんでした\n");
        return E_FAIL;
    }

    // ベース画像の情報取得
    const Image::ImageProperty& base_prop = image.getImageProperty(0);

    // ビットマップ画像を作る
    Microsoft::WRL::ComPtr<IWICBitmap> p_bitmap;
    HRESULT hr = mpFactory->CreateBitmapFromMemory(
        static_cast<UINT>(image.getWidth()),
        static_cast<UINT>(image.getHeight()),
        format_guid,
        static_cast<UINT>(base_prop.row_pitch),
        static_cast<UINT>(base_prop.slice_pitch),
        image.getMemoryPtr(base_prop.memory_offset),
        p_bitmap.GetAddressOf()
    );
    if (FAILED(hr))
    {
        BEL_ERROR_LOG("ミップマップ生成用画像の生成に失敗しました\n");
        BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
        return hr;
    }

    // 各ミップマップに併せてスケールを掛ける
    for (size_t level = 1; level < mip_levels; ++level)
    {
        //
        if (level >= image.getMipLevels())
        {
            break;
        }

        const Image::ImageProperty& prop = image.getImageProperty(level);

        // スケーラー初期化
        Microsoft::WRL::ComPtr<IWICBitmapScaler> p_scaler;
        hr = mpFactory->CreateBitmapScaler(p_scaler.GetAddressOf());
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("ビットマップスケーラーの生成に失敗しました\n");
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        // 縮小画像を生成
        hr = p_scaler->Initialize(
            p_bitmap.Get(),
            static_cast<UINT>(prop.width),
            static_cast<UINT>(prop.height),
            WICBitmapInterpolationModeCubic
        );
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("ミップマップの生成に失敗しました\n");
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        // 縮小画像のフォーマットを取得
        WICPixelFormatGUID scaled_format;
        hr = p_scaler->GetPixelFormat(&scaled_format);
        if (FAILED(hr))
        {
            BEL_ERROR_LOG("ミップマップのフォーマット取得に失敗しました\n");
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }

        if (memcmp(&scaled_format, &format_guid, sizeof(WICPixelFormatGUID)) == 0)
        {
            // フォーマットが同じならコピーするだけ
            hr = p_scaler->CopyPixels(
                nullptr,
                static_cast<UINT>(prop.row_pitch),
                static_cast<UINT>(prop.slice_pitch),
                image.getMemoryPtr(prop.memory_offset)
            );
            BEL_ERROR_LOG("ミップマップのコピーに失敗しました\n");
            BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
            return hr;
        }
        else
        {
            // フォーマットが異なれば変換してコピーする
            Microsoft::WRL::ComPtr<IWICFormatConverter> p_converter;
            hr = mpFactory->CreateFormatConverter(p_converter.GetAddressOf());
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("ミップマップのフォーマットコンバーターの初期化に失敗しました\n");
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            // 変換チェック
            BOOL can_convert = FALSE;
            hr = p_converter->CanConvert(scaled_format, format_guid, &can_convert);
            if (FAILED(hr) || !can_convert)
            {
                BEL_ERROR_LOG("ミップマップのフォーマットがコンバートできない組み合わせです\n");
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            // 変換
            hr = p_converter->Initialize(
                p_scaler.Get(),
                format_guid,
                WICBitmapDitherTypeNone,
                nullptr, 0,
                WICBitmapPaletteTypeMedianCut
            );
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("ミップマップのコンバートに失敗しました\n");
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }

            hr = p_converter->CopyPixels(
                nullptr,
                static_cast<UINT>(prop.row_pitch),
                static_cast<UINT>(prop.slice_pitch),
                image.getMemoryPtr(prop.memory_offset)
            );
            if (FAILED(hr))
            {
                BEL_ERROR_LOG("ミップマップのコピーに失敗しました\n");
                BEL_ERROR_LOG("  エラー: %s\n", std::system_category().message(hr).c_str());
                return hr;
            }
        }
    }

    return S_OK;
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
bool TextureConverter::FormatToGuid_(WICPixelFormatGUID& guid, gfx::TextureFormat format)
{
    switch (format)
    {
    case bel::gfx::TextureFormat::cR8_uNorm:
        guid = GUID_WICPixelFormat8bppGray;
        return true;
    case bel::gfx::TextureFormat::cA8_uNorm:
        guid = GUID_WICPixelFormat8bppAlpha;
        return true;
    case bel::gfx::TextureFormat::cR8G8B8A8_uNorm:
    case bel::gfx::TextureFormat::cR8G8B8A8_sRGB:
        guid = GUID_WICPixelFormat32bppRGBA;
        return true;
    case bel::gfx::TextureFormat::cR16_uNorm:
        guid = GUID_WICPixelFormat16bppGray;
        return true;
    case bel::gfx::TextureFormat::cR16_Float:
        guid = GUID_WICPixelFormat16bppGrayHalf;
        return true;
    case bel::gfx::TextureFormat::cR16G16B16A16_uNorm:
        guid = GUID_WICPixelFormat64bppRGBA;
        return true;
    case bel::gfx::TextureFormat::cR16G16B16A16_Float:
        guid = GUID_WICPixelFormat64bppRGBAHalf;
        return true;
    case bel::gfx::TextureFormat::cR32_Float:
        guid = GUID_WICPixelFormat32bppGrayFloat;
        return true;
    case bel::gfx::TextureFormat::cR32G32B32A32_Float:
        guid = GUID_WICPixelFormat128bppRGBAFloat;
        return true;
    case bel::gfx::TextureFormat::cR1_uNorm:
        guid = GUID_WICPixelFormatBlackWhite;
        return true;
    case bel::gfx::TextureFormat::cR5G6B5_uNorm:
        guid = GUID_WICPixelFormat16bppBGR565;
        return true;
    case bel::gfx::TextureFormat::cR5G5B5A1_uNorm:
        guid = GUID_WICPixelFormat16bppBGRA5551;
        return true;
    case bel::gfx::TextureFormat::cR10G10B10A2_uNorm:
        guid = GUID_WICPixelFormat32bppRGBA1010102;
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
}