/*!
 * @file   belImage.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsTextureFormatInfo.h"
#include "image/belImage.h"

namespace bel {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Image::initialize2D(const InitializeArg& arg)
{
    // メタデータをコピー
    mMetadata.width      = arg.width;
    mMetadata.height     = arg.height;
    mMetadata.array_size = arg.array_size;
    mMetadata.mip_levels = arg.mip_levels;
    mMetadata.format     = arg.format;

    mMetadata.depth     = 1;
    mMetadata.dimension = gfx::TextureDimension::c2D;

    // プロパティ作成
    mImageProperties = std::make_unique<ImageProperty[]>(mMetadata.mip_levels);

    // メモリー確保
    mMetadata.memory_size = computePixelSize_();
    mMemory = std::make_unique<uint8_t[]>(mMetadata.memory_size);

    return true;
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
size_t Image::computePixelSize_() const
{
    size_t total_pixel_size = 0;
    size_t w = mMetadata.width;
    size_t h = mMetadata.height;

    for (size_t i_level = 0; i_level < mMetadata.mip_levels; ++i_level)
    {
        auto [row_pitch, slice_pitch] = computePitch_(w, h);

        // データ記録
        mImageProperties[i_level].width         = w;
        mImageProperties[i_level].height        = h;
        mImageProperties[i_level].row_pitch     = row_pitch;
        mImageProperties[i_level].slice_pitch   = slice_pitch;
        mImageProperties[i_level].memory_offset = total_pixel_size;

        // 総容量計算
        total_pixel_size += slice_pitch;

        // 次のミップレベルのサイズを計算
        if (h > 1) { h >>= 1; }
        if (w > 1) { w >>= 1; }
    }

    return total_pixel_size;
}
//-----------------------------------------------------------------------------
std::pair<size_t, size_t> Image::computePitch_(size_t width, size_t height) const
{
    size_t pitch = 0;
    size_t slice = 0;

    switch (mMetadata.format)
    {
        // BC圧縮 4bpp
    case gfx::TextureFormat::cBC1_uNorm:
    case gfx::TextureFormat::cBC1_sRGB:
    case gfx::TextureFormat::cBC4_uNorm:
    case gfx::TextureFormat::cBC4_sNorm:
        BEL_ASSERT(gfx::TextureFormatInfo::IsCompressed(mMetadata.format));
        {
            // 4x4 ピクセルでタイルを構成する
            size_t tile_w = Math::Roundup(width, 4LLU);  // @TODO: LLU -> ZU
            size_t tile_h = Math::Roundup(height, 4LLU); // @TODO: LLU -> ZU
            // タイルあたり8ビット
            pitch = tile_w * 8;
            slice = pitch * tile_h;
        }
        break;

        // BC圧縮 8bpp
    case gfx::TextureFormat::cBC2_uNorm:
    case gfx::TextureFormat::cBC2_sRGB:
    case gfx::TextureFormat::cBC3_uNorm:
    case gfx::TextureFormat::cBC3_sRGB:
    case gfx::TextureFormat::cBC5_uNorm:
    case gfx::TextureFormat::cBC5_sNorm:
    case gfx::TextureFormat::cBC6_uFloat:
    case gfx::TextureFormat::cBC6_Float:
    case gfx::TextureFormat::cBC7_uNorm:
    case gfx::TextureFormat::cBC7_sRGB:
        BEL_ASSERT(gfx::TextureFormatInfo::IsCompressed(mMetadata.format));
        {
            // 4x4 ピクセルでタイルを構成する
            size_t tile_w = Math::Roundup(width, 4LLU);  // @TODO: LLU -> ZU
            size_t tile_h = Math::Roundup(height, 4LLU); // @TODO: LLU -> ZU
            // タイルあたり16ビット
            pitch = tile_w * 16;
            slice = pitch * tile_h;
        }
        break;

    default:
        BEL_ASSERT(!gfx::TextureFormatInfo::IsCompressed(mMetadata.format));
        pitch = Math::Roundup(width * gfx::TextureFormatInfo::BitsPerPixel(mMetadata.format), 8LLU); // @TODO: LLU -> ZU
        slice = pitch * height;
        break;
    }

    return { pitch / 8, slice / 8 };
}
//-----------------------------------------------------------------------------
}