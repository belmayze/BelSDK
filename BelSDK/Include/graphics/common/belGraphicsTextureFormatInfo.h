/*!
 * @file   belGraphicsTextureFormatInfo.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "common/belEnumFlags.h"
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
    static constexpr bool IsDepth(TextureFormat f) { return cProperties[static_cast<size_t>(f)].flags.test(FormatFlag::cDepth); }
    //! 圧縮フォーマット
    static constexpr bool IsCompressed(TextureFormat f) { return cProperties[static_cast<size_t>(f)].flags.test(FormatFlag::cCompressed); }
    //! bpp
    static constexpr size_t BitsPerPixel(TextureFormat f) { return cProperties[static_cast<size_t>(f)].bpp; }

    //-------------------------------------------------------------------------
private:
    //! フラグ
    enum class FormatFlag : uint8_t
    {
        cDepth,      //!< デプスフォーマット
        cCompressed, //!< 圧縮フォーマット

        cNum
    };

    //! テクスチャー情報クラス
    struct Property
    {
        EnumFlags<FormatFlag> flags;   //!< フラグ
        size_t                bpp = 0; //!< ピクセル当たりのビット数
    };

    //! テクスチャー情報
    static constexpr auto cProperties = []
    {
        std::array<Property, static_cast<size_t>(TextureFormat::cNum)> arr;
        for (uint32_t i = 0; i < arr.size(); ++i)
        {
            switch (static_cast<TextureFormat>(i))
            {
                // ----------------------------------------
                // 8bit カラー
            case TextureFormat::cR8_uNorm:
            case TextureFormat::cR8_sNorm:
            case TextureFormat::cR8_uInt:
            case TextureFormat::cR8_sInt:
            case TextureFormat::cA8_uNorm:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 8;
                break;

            case TextureFormat::cR8G8_uNorm:
            case TextureFormat::cR8G8_sNorm:
            case TextureFormat::cR8G8_uInt:
            case TextureFormat::cR8G8_sInt:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 16;
                break;

            case TextureFormat::cR8G8B8A8_uNorm:
            case TextureFormat::cR8G8B8A8_sNorm:
            case TextureFormat::cR8G8B8A8_sRGB:
            case TextureFormat::cR8G8B8A8_uInt:
            case TextureFormat::cR8G8B8A8_sInt:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 32;
                break;

                // ----------------------------------------
                // 16ビットカラー
            case TextureFormat::cR16_uNorm:
            case TextureFormat::cR16_sNorm:
            case TextureFormat::cR16_uInt:
            case TextureFormat::cR16_sInt:
            case TextureFormat::cR16_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 16;
                break;

            case TextureFormat::cR16G16_uNorm:
            case TextureFormat::cR16G16_sNorm:
            case TextureFormat::cR16G16_uInt:
            case TextureFormat::cR16G16_sInt:
            case TextureFormat::cR16G16_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 32;
                break;

            case TextureFormat::cR16G16B16A16_uNorm:
            case TextureFormat::cR16G16B16A16_sNorm:
            case TextureFormat::cR16G16B16A16_uInt:
            case TextureFormat::cR16G16B16A16_sInt:
            case TextureFormat::cR16G16B16A16_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 64;
                break;

                // ----------------------------------------
                // 32ビットカラー
            case TextureFormat::cR32_uInt:
            case TextureFormat::cR32_sInt:
            case TextureFormat::cR32_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 32;
                break;

            case TextureFormat::cR32G32_uInt:
            case TextureFormat::cR32G32_sInt:
            case TextureFormat::cR32G32_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 64;
                break;

            case TextureFormat::cR32G32B32_uInt:
            case TextureFormat::cR32G32B32_sInt:
            case TextureFormat::cR32G32B32_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 96;
                break;

            case TextureFormat::cR32G32B32A32_uInt:
            case TextureFormat::cR32G32B32A32_sInt:
            case TextureFormat::cR32G32B32A32_Float:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 128;
                break;

                // ----------------------------------------
                // その他カラー
            case TextureFormat::cR1_uNorm:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 1;
                break;

            case TextureFormat::cR5G6B5_uNorm:
            case TextureFormat::cR5G5B5A1_uNorm:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 16;
                break;

            case TextureFormat::cR10G10B10A2_uNorm:
            case TextureFormat::cR10G10B10A2_uInt:
            case TextureFormat::cR11G11B10_uFloat:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 32;
                break;

                // ----------------------------------------
                // デプス
            case TextureFormat::cD16_uNorm:
                arr[i].flags.set(FormatFlag::cDepth, true);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 16;
                break;

            case TextureFormat::cD24_uNorm_S8_uInt:
            case TextureFormat::cD32_Float:
                arr[i].flags.set(FormatFlag::cDepth, true);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 32;
                break;

            case TextureFormat::cD32_Float_S8_uInt:
                arr[i].flags.set(FormatFlag::cDepth, true);
                arr[i].flags.set(FormatFlag::cCompressed, false);
                arr[i].bpp = 64;
                break;

                // ----------------------------------------
                // BC圧縮
            case TextureFormat::cBC1_uNorm:
            case TextureFormat::cBC1_sRGB:
            case TextureFormat::cBC4_uNorm:
            case TextureFormat::cBC4_sNorm:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, true);
                arr[i].bpp = 4;
                break;

            case TextureFormat::cBC2_uNorm:
            case TextureFormat::cBC2_sRGB:
            case TextureFormat::cBC3_uNorm:
            case TextureFormat::cBC3_sRGB:
            case TextureFormat::cBC5_uNorm:
            case TextureFormat::cBC5_sNorm:
            case TextureFormat::cBC6_uFloat:
            case TextureFormat::cBC6_Float:
            case TextureFormat::cBC7_uNorm:
            case TextureFormat::cBC7_sRGB:
                arr[i].flags.set(FormatFlag::cDepth, false);
                arr[i].flags.set(FormatFlag::cCompressed, true);
                arr[i].bpp = 8;
                break;
            }
        }
        return arr;
    }();
};
//-----------------------------------------------------------------------------
}
