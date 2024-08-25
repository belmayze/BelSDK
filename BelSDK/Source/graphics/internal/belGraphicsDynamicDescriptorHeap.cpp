/*!
 * @file   belGraphicsDynamicDescriptorHeap.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/internal/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
bool DynamicDescriptorHeap::initialize(uint32_t num)
{
    // グローバルで大きなテクスチャー用デスクリプターヒープを作る
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = num * 2; // ダブルバッファ
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mpDescriptorHeap))))
        {
            return false;
        }
    }
    mDescriptorSize = num;

    return true;
}
//-----------------------------------------------------------------------------
}