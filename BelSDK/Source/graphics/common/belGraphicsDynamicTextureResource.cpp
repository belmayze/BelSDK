/*!
 * @file   belGraphicsDynamicTextureResource.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDynamicTextureResource.h"
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

    return true;
}
//-----------------------------------------------------------------------------
}