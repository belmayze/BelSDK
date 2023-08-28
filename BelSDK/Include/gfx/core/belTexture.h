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

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * テクスチャー
 */
class Texture
{
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
     * @param[in] num_mip    ミップマップ数
     * @param[in] format     フォーマット
     * @param[in] p_resource リソース
     */
    void initializeFromResource(uint32_t width, uint32_t height, uint32_t num_mip, DXGI_FORMAT format, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 横幅
    uint32_t getWidth() const { return mWidth; }
    //! 高さ
    uint32_t getHeight() const { return mHeight; }
    //! ミップマップ数
    uint32_t getNumMip() const { return mNumMip; }
    //! フォーマット
    DXGI_FORMAT getFormat() const { return mFormat; }

    //
    ID3D12Resource& getResource() const { BEL_ASSERT(mpResource); return *mpResource.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mpResource;

    uint32_t    mWidth  = 0;
    uint32_t    mHeight = 0;
    uint32_t    mNumMip = 0;
    DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
};
//-----------------------------------------------------------------------------

}
