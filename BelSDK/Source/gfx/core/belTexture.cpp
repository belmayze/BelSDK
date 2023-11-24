/*!
 * @file   belTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belTexture.h"
#include "gfx/core/belTextureDescriptorRegistry.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool Texture::initialize2D(uint32_t width, uint32_t height, uint32_t num_mip, DXGI_FORMAT format, void* buffer_ptr, D3D12_RESOURCE_FLAGS flags)
{
    // テクスチャーを作成
    Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension        = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Width            = width;
    desc.Height           = height;
    desc.MipLevels        = num_mip;
    desc.Format           = format;
    desc.SampleDesc.Count = 1;
    desc.Flags            = flags;

    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type                 = D3D12_HEAP_TYPE_DEFAULT;
    prop.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask     = 1;
    prop.VisibleNodeMask      = 1;

    if (FAILED(Graphics::GetInstance().getDevice().CreateCommittedResource(
        &prop, D3D12_HEAP_FLAG_NONE,
        &desc, D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&p_resource)
    )))
    {
        return false;
    }

    // バッファーがあればコピーする
    if (buffer_ptr != nullptr)
    {
        // @TODO: GPUコピーコマンドを発行する
    }

    mWidth     = width;
    mHeight    = height;
    mDepth     = 1;
    mNumMip    = num_mip;
    mFormat    = format;
    mDimension = cDimension2D;
    mpResource = std::move(p_resource);

    // デスクリプターヒープに登録する
    mDescriptorHandle = TextureDescriptorRegistry::GetInstance().registerTexture(*this);

    return true;
}
//-----------------------------------------------------------------------------
void Texture::initializeFromResource(uint32_t width, uint32_t height, uint32_t depth, uint32_t num_mip, DXGI_FORMAT format, Dimension dimension, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource)
{
    mWidth     = width;
    mHeight    = height;
    mDepth     = depth;
    mNumMip    = num_mip;
    mFormat    = format;
    mDimension = dimension;
    mpResource = std::move(p_resource);

    // デスクリプターヒープに登録する
    mDescriptorHandle = TextureDescriptorRegistry::GetInstance().registerTexture(*this);
}
//-----------------------------------------------------------------------------
}
