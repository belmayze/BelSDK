/*!
 * @file   belGraphicsDynamicTextureResource.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDynamicTextureResource.h"
#include "graphics/common/belGraphicsTextureFormatInfo.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
bool DynamicTextureResource::initialize(size_t size)
{
    // GPUメモリーを確保する
    D3D12_HEAP_DESC desc = {};
    desc.SizeInBytes = size;
    desc.Alignment   = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    desc.Flags       = D3D12_HEAP_FLAG_ALLOW_ONLY_RT_DS_TEXTURES;
    desc.Properties.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    desc.Properties.CreationNodeMask     = 1;
    desc.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    desc.Properties.Type                 = D3D12_HEAP_TYPE_DEFAULT;
    desc.Properties.VisibleNodeMask      = 1;
    if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateHeap(&desc, IID_PPV_ARGS(&mpHeap))))
    {
        return false;
    }
    mHeapSize = size;

    // アロケーター
    mAllocator.initialize(size, 1024);
    mAllocatedList.allocate(1024);

    return true;
}
//-----------------------------------------------------------------------------
Texture DynamicTextureResource::allocate(const AllocateArg& arg)
{
    // Resource作成
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

    // リソースサイズ計算
    D3D12_RESOURCE_ALLOCATION_INFO alloc_info = GraphicsEngine::GetInstance().getDevice().GetResourceAllocationInfo(
        0, 1, &desc
    );

    // 必要なリソースサイズの空きを検索
    auto offset = mAllocator.allocate(alloc_info.SizeInBytes, alloc_info.Alignment);
    if (!offset.has_value())
    {
        // 空きがない場合は無効
        return Texture();
    }

    // クリアカラーの最適化
    D3D12_CLEAR_VALUE clear_value;
    clear_value.Format = desc.Format;
    if (TextureFormatInfo::IsDepth(arg.format))
    {
        clear_value.DepthStencil.Depth   = arg.optimized_clear_depth;
        clear_value.DepthStencil.Stencil = arg.optimized_clear_stencil;
    }
    else
    {
        clear_value.Color[0] = arg.optimized_clear_color.r();
        clear_value.Color[1] = arg.optimized_clear_color.g();
        clear_value.Color[2] = arg.optimized_clear_color.b();
        clear_value.Color[3] = arg.optimized_clear_color.a();
    }
    
    // リソース確保
    Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
    if (FAILED(
        GraphicsEngine::GetInstance().getDevice().CreatePlacedResource(
            mpHeap.Get(), offset.value(), &desc,
            D3D12_RESOURCE_STATE_COPY_DEST,
            &clear_value, IID_PPV_ARGS(&p_resource)
        )
    ))
    {
        BEL_ERROR_LOG("ダイナミックテクスチャーの確保に失敗しました\n");
        return Texture();
    }

    // 確保したリソースでテクスチャーを確保
    Texture texture;
    {
        Texture::InitializeArg init_arg;
        init_arg.width = arg.width;
        init_arg.height = arg.height;
        init_arg.depth = arg.depth;
        init_arg.mip_levels = arg.mip_levels;
        init_arg.component_mapping = arg.component_mapping;
        init_arg.format = arg.format;
        init_arg.dimension = arg.dimension;
        init_arg.optimized_clear_color = arg.optimized_clear_color;
        init_arg.optimized_clear_depth = arg.optimized_clear_depth;
        init_arg.optimized_clear_stencil = arg.optimized_clear_stencil;
        if (!texture.initializeFromGPUMemory(init_arg, std::move(p_resource), ResourceState::cCopyDest))
        {
            return Texture();
        }
    }

    // リソースとオフセットの紐づけ
    mAllocatedList.emplace_back(&texture.getResource(), offset.value());

    return texture;
}
//-----------------------------------------------------------------------------
void DynamicTextureResource::free(Texture&& texture)
{
    // リソースからオフセットを検索
    auto it = std::find_if(mAllocatedList.begin(), mAllocatedList.end(), [&texture](const MemoryKeyValue& pair)
        {
            return pair.first == &texture.getResource();
        });
    BEL_ASSERT(it != mAllocatedList.end());

    // 指定したオフセットを解放
    mAllocator.free(it->second);
    mAllocatedList.erase(it);
}
//-----------------------------------------------------------------------------
}