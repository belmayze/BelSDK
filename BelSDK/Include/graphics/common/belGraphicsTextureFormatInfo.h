/*!
 * @file   belGraphicsTextureFormatInfo.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// テクスチャーフォーマット情報
//-----------------------------------------------------------------------------
class TextureFormatInfo
{
    //-------------------------------------------------------------------------
public:
    //! デプスフォーマット
    static constexpr bool isDepthFormat(TextureFormat f) { return cProperties[static_cast<size_t>(f)].is_depth; }

    //-------------------------------------------------------------------------
private:
    //! テクスチャー情報クラス
    struct Property
    {
        bool is_depth = false;
    };

    //! テクスチャー情報
    static constexpr auto cProperties = []
    {
        std::array<Property, static_cast<size_t>(TextureFormat::cNum)> arr;
        for (uint32_t i = 0; i < arr.size(); ++i)
        {
            switch (static_cast<TextureFormat>(i))
            {
            case TextureFormat::cR8G8B8A8_uNorm:
                arr[i].is_depth = false;
                break;
            case TextureFormat::cR8G8B8A8_sRGB:
                arr[i].is_depth = false;
                break;
            case TextureFormat::cR10G10B10A2_uNorm:
                arr[i].is_depth = false;
                break;
            case TextureFormat::cR11G11B10_Float:
                arr[i].is_depth = false;
                break;
            case TextureFormat::cD16_uNorm:
                arr[i].is_depth = true;
                break;
            case TextureFormat::cD24_uNorm_S8_uInt:
                arr[i].is_depth = true;
                break;
            case TextureFormat::cD32_Float:
                arr[i].is_depth = true;
                break;
            case TextureFormat::cD32_Float_S8_uInt:
                arr[i].is_depth = true;
                break;
            }
        }
        return arr;
    }();
};
//-----------------------------------------------------------------------------
}
