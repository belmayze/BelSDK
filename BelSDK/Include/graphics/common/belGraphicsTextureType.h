/*!
 * @file   belGraphicsTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// テクスチャーフォーマット
//-----------------------------------------------------------------------------
enum class TextureFormat
{
    cR8G8B8A8_uNorm, //!< RGBA 32ビット符号なし正規化整数
    cR8G8B8A8_sRGB,  //!< RGBA 32ビット符号なし正規化整数（sRGB）
    cNum
};

// cast
constexpr DXGI_FORMAT to_native(TextureFormat format)
{
    switch (format)
    {
    case bel::gfx::TextureFormat::cR8G8B8A8_uNorm: return DXGI_FORMAT_R8G8B8A8_UNORM;
    case bel::gfx::TextureFormat::cR8G8B8A8_sRGB:  return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    }

    BEL_ASSERT(0);
    return DXGI_FORMAT_R8G8B8A8_UNORM;
}

//-----------------------------------------------------------------------------
// テクスチャーの次元
//-----------------------------------------------------------------------------
enum class TextureDimension
{
    c1D,        //!< 1次元
    c1DArray,   //!< 1次元配列
    c2D,        //!< 2次元
    c2DArray,   //!< 2次元配列
    c3D,        //!< 3次元
    cCube,      //!< キューブマップ
    cCubeArray, //!< キューブマップ配列
    cNum
};

// cast
constexpr D3D12_SRV_DIMENSION to_native(TextureDimension dimension)
{
    switch (dimension)
    {
    case bel::gfx::TextureDimension::c1D:        return D3D12_SRV_DIMENSION_TEXTURE1D;
    case bel::gfx::TextureDimension::c1DArray:   return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
    case bel::gfx::TextureDimension::c2D:        return D3D12_SRV_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::c2DArray:   return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
    case bel::gfx::TextureDimension::c3D:        return D3D12_SRV_DIMENSION_TEXTURE3D;
    case bel::gfx::TextureDimension::cCube:      return D3D12_SRV_DIMENSION_TEXTURECUBE;
    case bel::gfx::TextureDimension::cCubeArray: return D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
    }

    BEL_ASSERT(0);
    return D3D12_SRV_DIMENSION_TEXTURE1D;
}

//-----------------------------------------------------------------------------
}
