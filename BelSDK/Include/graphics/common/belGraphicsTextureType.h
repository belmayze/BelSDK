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
    cR8G8B8A8_uNorm,    //!< RGBA 32ビット符号なし正規化整数
    cR8G8B8A8_sRGB,     //!< RGBA 32ビット符号なし正規化整数（sRGB）
    cR10G10B10A2_uNorm, //!< RGBA 32ビット符号なし正規化整数
    cR11G11B10_Float,   //!< RGB  32ビット符号なし浮動小数

    cD16_uNorm,         //!< 16ビット正規化整数デプス
    cD24_uNorm_S8_uInt, //!< 24ビット正規化整数デプス + 8ビット整数ステンシル
    cD32_Float,         //!< 32ビット浮動小数デプス
    cD32_Float_S8_uInt, //!< 32ビット浮動小数デプス + 8ビット整数ステンシル
    cNum
};

// cast
constexpr DXGI_FORMAT to_native(TextureFormat format)
{
    switch (format)
    {
    case bel::gfx::TextureFormat::cR8G8B8A8_uNorm:    return DXGI_FORMAT_R8G8B8A8_UNORM;
    case bel::gfx::TextureFormat::cR8G8B8A8_sRGB:     return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case bel::gfx::TextureFormat::cR10G10B10A2_uNorm: return DXGI_FORMAT_R10G10B10A2_UNORM;
    case bel::gfx::TextureFormat::cR11G11B10_Float:   return DXGI_FORMAT_R11G11B10_FLOAT;
    case bel::gfx::TextureFormat::cD16_uNorm:         return DXGI_FORMAT_D16_UNORM;
    case bel::gfx::TextureFormat::cD24_uNorm_S8_uInt: return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case bel::gfx::TextureFormat::cD32_Float:         return DXGI_FORMAT_D32_FLOAT;
    case bel::gfx::TextureFormat::cD32_Float_S8_uInt: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
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
constexpr D3D12_RESOURCE_DIMENSION to_native(TextureDimension dimension)
{
    switch (dimension)
    {
    case bel::gfx::TextureDimension::c1D:        return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
    case bel::gfx::TextureDimension::c1DArray:   return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
    case bel::gfx::TextureDimension::c2D:        return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::c2DArray:   return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::c3D:        return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
    case bel::gfx::TextureDimension::cCube:      return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::cCubeArray: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    }

    BEL_ASSERT(0);
    return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
}
constexpr D3D12_SRV_DIMENSION to_native_srv(TextureDimension dimension)
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
constexpr D3D12_RTV_DIMENSION to_native_rtv(TextureDimension dimension)
{
    switch (dimension)
    {
    case bel::gfx::TextureDimension::c1D:        return D3D12_RTV_DIMENSION_TEXTURE1D;
    case bel::gfx::TextureDimension::c1DArray:   return D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
    case bel::gfx::TextureDimension::c2D:        return D3D12_RTV_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::c2DArray:   return D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
    case bel::gfx::TextureDimension::c3D:        return D3D12_RTV_DIMENSION_TEXTURE3D;
    }

    BEL_ASSERT(0);
    return D3D12_RTV_DIMENSION_TEXTURE1D;
}
constexpr D3D12_DSV_DIMENSION to_native_dsv(TextureDimension dimension)
{
    switch (dimension)
    {
    case bel::gfx::TextureDimension::c1D:        return D3D12_DSV_DIMENSION_TEXTURE1D;
    case bel::gfx::TextureDimension::c1DArray:   return D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
    case bel::gfx::TextureDimension::c2D:        return D3D12_DSV_DIMENSION_TEXTURE2D;
    case bel::gfx::TextureDimension::c2DArray:   return D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
    }

    BEL_ASSERT(0);
    return D3D12_DSV_DIMENSION_TEXTURE1D;
}

//-----------------------------------------------------------------------------
// リソースステート
//-----------------------------------------------------------------------------
enum class ResourceState
{
    cRenderTarget,
    cDepthWrite,
    cGenericRead,
    cPresent,
    cNum
};

// cast
constexpr D3D12_RESOURCE_STATES to_native(ResourceState state)
{
    switch (state)
    {
    case bel::gfx::ResourceState::cRenderTarget: return D3D12_RESOURCE_STATE_RENDER_TARGET;
    case bel::gfx::ResourceState::cDepthWrite:   return D3D12_RESOURCE_STATE_DEPTH_WRITE;
    case bel::gfx::ResourceState::cGenericRead:  return D3D12_RESOURCE_STATE_GENERIC_READ;
    case bel::gfx::ResourceState::cPresent:      return D3D12_RESOURCE_STATE_PRESENT;
    }

    BEL_ASSERT(0);
    return D3D12_RESOURCE_STATE_GENERIC_READ;
}

//-----------------------------------------------------------------------------
}
