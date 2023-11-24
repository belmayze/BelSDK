/*!
 * @file   belTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <d3d12.h>
#include <wrl/client.h>
// bel
#include "gfx/core/belTextureDescriptorHandle.h"

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * テクスチャー
 */
class Texture
{
    //-------------------------------------------------------------------------
public:
    enum Dimension
    {
        cDimensionInvalid,
        cDimension1D,
        cDimension2D,
        cDimension3D,
        cDimensionCube
    };

    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] width      横幅
     * @param[in] height     高さ
     * @param[in] num_mip    ミップマップ数
     * @param[in] format     フォーマット
     * @param[in] buffer_ptr バッファー
     * @param[in] flags      リソースフラグ
     */
    bool initialize2D(uint32_t width, uint32_t height, uint32_t num_mip, DXGI_FORMAT format, void* buffer_ptr = nullptr, D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

    /*!
     * 初期化（GPUメモリーを使う）
     * @param[in] width      横幅
     * @param[in] height     高さ
     * @param[in] depth      深さ
     * @param[in] num_mip    ミップマップ数
     * @param[in] format     フォーマット
     * @param[in] dimension  次元
     * @param[in] p_resource リソース
     */
    void initializeFromResource(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip, DXGI_FORMAT format, Dimension dimension, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 横幅
    uint32_t getWidth() const { return mWidth; }
    //! 高さ
    uint32_t getHeight() const { return mHeight; }
    //! 深さ
    uint32_t getDepth() const { return mDepth; }
    //! ミップマップ数
    uint32_t getNumMip() const { return mNumMip; }
    //! フォーマット
    DXGI_FORMAT getFormat() const { return mFormat; }
    //! 次元
    Dimension getDimension() const { return mDimension; }

    //
    ID3D12Resource& getResource() const { BEL_ASSERT(mpResource); return *mpResource.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mpResource;
    TextureDescriptorHandle                mDescriptorHandle;

    uint32_t    mWidth     = 0;
    uint32_t    mHeight    = 0;
    uint32_t    mDepth     = 0;
    uint32_t    mNumMip    = 0;
    Dimension   mDimension = cDimensionInvalid;
    DXGI_FORMAT mFormat    = DXGI_FORMAT_UNKNOWN;
};
//-----------------------------------------------------------------------------

}
