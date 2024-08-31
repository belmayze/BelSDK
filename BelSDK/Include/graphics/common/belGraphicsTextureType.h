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

// cast
constexpr DXGI_FORMAT to_native(TextureFormat format)
{
    switch (format)
    {
        // 8bit カラー
    case bel::gfx::TextureFormat::cR8_uNorm:            return DXGI_FORMAT_R8_UNORM;
    case bel::gfx::TextureFormat::cR8_sNorm:            return DXGI_FORMAT_R8_SNORM;
    case bel::gfx::TextureFormat::cR8_uInt:             return DXGI_FORMAT_R8_UINT;
    case bel::gfx::TextureFormat::cR8_sInt:             return DXGI_FORMAT_R8_SINT;
    case bel::gfx::TextureFormat::cA8_uNorm:            return DXGI_FORMAT_A8_UNORM;
    case bel::gfx::TextureFormat::cR8G8_uNorm:          return DXGI_FORMAT_R8G8_UNORM;
    case bel::gfx::TextureFormat::cR8G8_sNorm:          return DXGI_FORMAT_R8G8_SNORM;
    case bel::gfx::TextureFormat::cR8G8_uInt:           return DXGI_FORMAT_R8G8_UINT;
    case bel::gfx::TextureFormat::cR8G8_sInt:           return DXGI_FORMAT_R8G8_SINT;
    case bel::gfx::TextureFormat::cR8G8B8A8_uNorm:      return DXGI_FORMAT_R8G8B8A8_UNORM;
    case bel::gfx::TextureFormat::cR8G8B8A8_sNorm:      return DXGI_FORMAT_R8G8B8A8_SNORM;
    case bel::gfx::TextureFormat::cR8G8B8A8_sRGB:       return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case bel::gfx::TextureFormat::cR8G8B8A8_uInt:       return DXGI_FORMAT_R8G8B8A8_UINT;
    case bel::gfx::TextureFormat::cR8G8B8A8_sInt:       return DXGI_FORMAT_R8G8B8A8_SINT;
        // 16ビットカラー
    case bel::gfx::TextureFormat::cR16_uNorm:           return DXGI_FORMAT_R16_UNORM;
    case bel::gfx::TextureFormat::cR16_sNorm:           return DXGI_FORMAT_R16_SNORM;
    case bel::gfx::TextureFormat::cR16_uInt:            return DXGI_FORMAT_R16_UINT;
    case bel::gfx::TextureFormat::cR16_sInt:            return DXGI_FORMAT_R16_SINT;
    case bel::gfx::TextureFormat::cR16_Float:           return DXGI_FORMAT_R16_FLOAT;
    case bel::gfx::TextureFormat::cR16G16_uNorm:        return DXGI_FORMAT_R16G16_UNORM;
    case bel::gfx::TextureFormat::cR16G16_sNorm:        return DXGI_FORMAT_R16G16_SNORM;
    case bel::gfx::TextureFormat::cR16G16_uInt:         return DXGI_FORMAT_R16G16_UINT;
    case bel::gfx::TextureFormat::cR16G16_sInt:         return DXGI_FORMAT_R16G16_SINT;
    case bel::gfx::TextureFormat::cR16G16_Float:        return DXGI_FORMAT_R16G16_FLOAT;
    case bel::gfx::TextureFormat::cR16G16B16A16_uNorm:  return DXGI_FORMAT_R16G16B16A16_UNORM;
    case bel::gfx::TextureFormat::cR16G16B16A16_sNorm:  return DXGI_FORMAT_R16G16B16A16_SNORM;
    case bel::gfx::TextureFormat::cR16G16B16A16_uInt:   return DXGI_FORMAT_R16G16B16A16_UINT;
    case bel::gfx::TextureFormat::cR16G16B16A16_sInt:   return DXGI_FORMAT_R16G16B16A16_SINT;
    case bel::gfx::TextureFormat::cR16G16B16A16_Float:  return DXGI_FORMAT_R16G16B16A16_FLOAT;
        // 32ビットカラー
    case bel::gfx::TextureFormat::cR32_uInt:            return DXGI_FORMAT_R32_UINT;
    case bel::gfx::TextureFormat::cR32_sInt:            return DXGI_FORMAT_R32_SINT;
    case bel::gfx::TextureFormat::cR32_Float:           return DXGI_FORMAT_R32_FLOAT;
    case bel::gfx::TextureFormat::cR32G32_uInt:         return DXGI_FORMAT_R32G32_UINT;
    case bel::gfx::TextureFormat::cR32G32_sInt:         return DXGI_FORMAT_R32G32_SINT;
    case bel::gfx::TextureFormat::cR32G32_Float:        return DXGI_FORMAT_R32G32_FLOAT;
    case bel::gfx::TextureFormat::cR32G32B32_uInt:      return DXGI_FORMAT_R32G32B32_UINT;
    case bel::gfx::TextureFormat::cR32G32B32_sInt:      return DXGI_FORMAT_R32G32B32_SINT;
    case bel::gfx::TextureFormat::cR32G32B32_Float:     return DXGI_FORMAT_R32G32B32_FLOAT;
    case bel::gfx::TextureFormat::cR32G32B32A32_uInt:   return DXGI_FORMAT_R32G32B32A32_UINT;
    case bel::gfx::TextureFormat::cR32G32B32A32_sInt:   return DXGI_FORMAT_R32G32B32A32_SINT;
    case bel::gfx::TextureFormat::cR32G32B32A32_Float:  return DXGI_FORMAT_R32G32B32A32_FLOAT;
        // その他カラー
    case bel::gfx::TextureFormat::cR5G6B5_uNorm:        return DXGI_FORMAT_B5G6R5_UNORM;
    case bel::gfx::TextureFormat::cR5G5B5A1_uNorm:      return DXGI_FORMAT_B5G5R5A1_UNORM;
    case bel::gfx::TextureFormat::cR10G10B10A2_uNorm:   return DXGI_FORMAT_R10G10B10A2_UNORM;
    case bel::gfx::TextureFormat::cR10G10B10A2_uInt:    return DXGI_FORMAT_R10G10B10A2_UINT;
    case bel::gfx::TextureFormat::cR11G11B10_uFloat:    return DXGI_FORMAT_R11G11B10_FLOAT;
        // デプス
    case bel::gfx::TextureFormat::cD16_uNorm:           return DXGI_FORMAT_D16_UNORM;
    case bel::gfx::TextureFormat::cD24_uNorm_S8_uInt:   return DXGI_FORMAT_D24_UNORM_S8_UINT;
    case bel::gfx::TextureFormat::cD32_Float:           return DXGI_FORMAT_D32_FLOAT;
    case bel::gfx::TextureFormat::cD32_Float_S8_uInt:   return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        // BC圧縮
    case bel::gfx::TextureFormat::cBC1_uNorm:           return DXGI_FORMAT_BC1_UNORM;
    case bel::gfx::TextureFormat::cBC1_sRGB:            return DXGI_FORMAT_BC1_UNORM_SRGB;
    case bel::gfx::TextureFormat::cBC2_uNorm:           return DXGI_FORMAT_BC2_UNORM;
    case bel::gfx::TextureFormat::cBC2_sRGB:            return DXGI_FORMAT_BC2_UNORM_SRGB;
    case bel::gfx::TextureFormat::cBC3_uNorm:           return DXGI_FORMAT_BC3_UNORM;
    case bel::gfx::TextureFormat::cBC3_sRGB:            return DXGI_FORMAT_BC3_UNORM_SRGB;
    case bel::gfx::TextureFormat::cBC4_uNorm:           return DXGI_FORMAT_BC4_UNORM;
    case bel::gfx::TextureFormat::cBC4_sNorm:           return DXGI_FORMAT_BC4_SNORM;
    case bel::gfx::TextureFormat::cBC5_uNorm:           return DXGI_FORMAT_BC5_UNORM;
    case bel::gfx::TextureFormat::cBC5_sNorm:           return DXGI_FORMAT_BC5_SNORM;
    case bel::gfx::TextureFormat::cBC6_uFloat:          return DXGI_FORMAT_BC6H_UF16;
    case bel::gfx::TextureFormat::cBC6_sFloat:          return DXGI_FORMAT_BC6H_SF16;
    case bel::gfx::TextureFormat::cBC7_uNorm:           return DXGI_FORMAT_BC7_UNORM;
    case bel::gfx::TextureFormat::cBC7_sRGB:            return DXGI_FORMAT_BC7_UNORM_SRGB;
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
