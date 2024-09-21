/*!
 * @file   belTextureConverter.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// windows
#include <wincodec.h>
// bel
#include "graphics/common/belGraphicsTextureType.h"

namespace bel { class Image; }

namespace bel {
//-----------------------------------------------------------------------------
// メッシュ
//-----------------------------------------------------------------------------
class TextureConverter
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    TextureConverter();

    //! デストラクター
    ~TextureConverter();

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     */
    HRESULT initialize();

    //-------------------------------------------------------------------------
    // Read / Write
    //-------------------------------------------------------------------------
public:
    /*!
     * 画像ファイルを読み込みます
     * @param[out] image    読み込んだ画像ファイル
     * @param[in]  filepath 読み込みファイルパス
     */
    HRESULT readFile(Image& image, const std::string& filepath);

    //-------------------------------------------------------------------------
    // Convert
    //-------------------------------------------------------------------------
public:

    //-------------------------------------------------------------------------
private:
    struct ImageProperty
    {
        size_t width      = 0;
        size_t height     = 0;
        size_t depth      = 0; //!< 3D テクスチャー用
        size_t array_size = 0; //!< キューブマップなら6の倍数
        size_t mip_levels = 0;
        gfx::TextureFormat    format       = gfx::TextureFormat::cUnknown;
        gfx::TextureDimension dimension    = gfx::TextureDimension::c2D;
        WICPixelFormatGUID file_format_guid    = GUID_NULL; //!< ファイルフォーマットの GUID
        GUID               convert_format_guid = GUID_NULL; //!< 変換先のフォーマット GUID
    };

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<IWICImagingFactory2> mpFactory;
};
//-----------------------------------------------------------------------------
}
