/*!
 * @file   belGraphicsTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/common/belGraphicsTextureFormatInfo.h"
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Texture::initialize(const InitializeArg& arg)
{
    // リソースフープ設定
    D3D12_HEAP_PROPERTIES props = {};
    props.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.CreationNodeMask     = 1;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.Type                 = D3D12_HEAP_TYPE_DEFAULT;
    props.VisibleNodeMask      = 1;

    // リソース情報
    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension        = to_native(arg.mDimension);
    desc.Width            = arg.mWidth;
    desc.Height           = arg.mHeight;
    desc.DepthOrArraySize = arg.mDepth;
    desc.MipLevels        = arg.mNumMip;
    desc.Format           = to_native(arg.mFormat);
    desc.SampleDesc.Count = 1;
    desc.Flags            = TextureFormatInfo::isDepthFormat(arg.mFormat) ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
                                                                          : D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

    // リソース生成
    Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
    if (FAILED(
        GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE, &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr, IID_PPV_ARGS(&p_resource)
        )
    ))
    {
        return false;
    }

    return initializeFromGPUMemory(arg, std::move(p_resource), ResourceState::cGenericRead);
}
//-----------------------------------------------------------------------------
bool Texture::initializeFromGPUMemory(const InitializeArg& arg, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource, ResourceState state)
{
    // GPU メモリーなので記録するだけ
    mWidth         = arg.mWidth;
    mHeight        = arg.mHeight;
    mNumMip        = arg.mNumMip;
    mFormat        = arg.mFormat;
    mDimension     = arg.mDimension;
    mResourceState = state;
    mpResource     = std::move(p_resource);

    // レジスターに登録
    mDescriptorHandle = TextureDescriptorRegistry::GetInstance().registerTexture(*this);

    return true;
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Texture::barrierTransition(CommandContext& command, ResourceState next_state) const
{
    // ステートが異なる場合のみバリアを張る
    if (mResourceState != next_state)
    {
        D3D12_RESOURCE_BARRIER desc = {};
        desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        desc.Transition.pResource   = mpResource.Get();
        desc.Transition.StateBefore = to_native(mResourceState);
        desc.Transition.StateAfter  = to_native(next_state);
        command.getCommandList().ResourceBarrier(1, &desc);

        mResourceState = next_state;
    }
}
//-----------------------------------------------------------------------------
}