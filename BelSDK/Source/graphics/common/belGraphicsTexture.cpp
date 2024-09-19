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
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceTexture.h"

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
    desc.Dimension        = to_native(arg.dimension);
    desc.Width            = arg.width;
    desc.Height           = arg.height;
    desc.DepthOrArraySize = arg.depth;
    desc.MipLevels        = arg.mip_levels;
    desc.Format           = to_native(arg.format);
    desc.SampleDesc.Count = 1;
    desc.Flags            = TextureFormatInfo::IsDepth(arg.format) ? D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
                                                                   : D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    
    // クリアカラーの最適化
    D3D12_CLEAR_VALUE clear_value;
    clear_value.Format = desc.Format;
    if (TextureFormatInfo::IsDepth(arg.format))
    {
        clear_value.DepthStencil.Depth   = 1.f;
        clear_value.DepthStencil.Stencil = 0;
    }
    else
    {
        clear_value.Color[0] = 0.f;
        clear_value.Color[1] = 0.f;
        clear_value.Color[2] = 0.f;
        clear_value.Color[3] = 1.f;
    }

    // リソース生成
    Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
    if (FAILED(
        GraphicsEngine::GetInstance().getDevice().CreateCommittedResource(
            &props, D3D12_HEAP_FLAG_NONE, &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            &clear_value, IID_PPV_ARGS(&p_resource)
        )
    ))
    {
        return false;
    }

    return initializeFromGPUMemory(arg, std::move(p_resource), ResourceState::cGenericRead);
}
//-----------------------------------------------------------------------------
bool Texture::initialize(const res::Texture& resource)
{
    // リソーステクスチャーから arg を生成
    {
        InitializeArg init_arg;
        init_arg.width      = resource.getWidth();
        init_arg.height     = resource.getHeight();
        init_arg.depth      = resource.getDepth();
        init_arg.mip_levels = resource.getMipLevels();
        init_arg.format     = resource.getFormat();
        init_arg.dimension  = resource.getDimension();
        if (!initialize(init_arg)) { return false; }
    }

    // メモリーコピー
    // @TODO: コピーコマンドからコピーする方法がいい
    {
        uint8_t* memory_ptr = nullptr;
        mpResource->Map(0, nullptr, reinterpret_cast<void**>(&memory_ptr));
        {
            mpResource->WriteToSubresource(
                0, nullptr,
                resource.getImageMemoryPtr(),
                TextureFormatInfo::BitsPerPixel(resource.getFormat()) / 8 * resource.getWidth(),
                resource.getImageMemorySize()
            );
        }
        mpResource->Unmap(0, nullptr);
    }

    return true;
}
//-----------------------------------------------------------------------------
bool Texture::initializeFromGPUMemory(const InitializeArg& arg, Microsoft::WRL::ComPtr<ID3D12Resource>&& p_resource, ResourceState state)
{
    // GPU メモリーなので記録するだけ
    mWidth         = arg.width;
    mHeight        = arg.height;
    mMipLevels     = arg.mip_levels;
    mFormat        = arg.format;
    mDimension     = arg.dimension;
    mResourceState = state;
    mpResource     = std::move(p_resource);

    // デスクリプターに登録
    mDescriptorHandle = GlobalDescriptorRegistry::GetInstance().registerTexture(*this);

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