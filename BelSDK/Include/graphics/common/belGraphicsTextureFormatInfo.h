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
                // 8bit カラー
            case bel::gfx::TextureFormat::cR8_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cA8_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8B8A8_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8B8A8_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8B8A8_sRGB:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8B8A8_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR8G8B8A8_sInt:
                arr[i].is_depth = false;
                break;
                // 16ビットカラー
            case bel::gfx::TextureFormat::cR16_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16_Float:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16_Float:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16B16A16_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16B16A16_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16B16A16_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16B16A16_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR16G16B16A16_Float:
                arr[i].is_depth = false;
                break;
                // 32ビットカラー
            case bel::gfx::TextureFormat::cR32_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32_Float:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32_Float:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32_Float:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32A32_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32A32_sInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR32G32B32A32_Float:
                arr[i].is_depth = false;
                break;
                // その他カラー
            case bel::gfx::TextureFormat::cR5G6B5_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR5G5B5A1_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR10G10B10A2_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR10G10B10A2_uInt:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cR11G11B10_uFloat:
                arr[i].is_depth = false;
                break;
                // デプス
            case bel::gfx::TextureFormat::cD16_uNorm:
                arr[i].is_depth = true;
                break;
            case bel::gfx::TextureFormat::cD24_uNorm_S8_uInt:
                arr[i].is_depth = true;
                break;
            case bel::gfx::TextureFormat::cD32_Float:
                arr[i].is_depth = true;
                break;
            case bel::gfx::TextureFormat::cD32_Float_S8_uInt:
                arr[i].is_depth = true;
                break;
                // BC圧縮
            case bel::gfx::TextureFormat::cBC1_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC1_sRGB:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC2_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC2_sRGB:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC3_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC3_sRGB:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC4_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC4_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC5_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC5_sNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC6_uFloat:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC6_sFloat:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC7_uNorm:
                arr[i].is_depth = false;
                break;
            case bel::gfx::TextureFormat::cBC7_sRGB:
                arr[i].is_depth = false;
                break;
            }
        }
        return arr;
    }();
};
//-----------------------------------------------------------------------------
}
