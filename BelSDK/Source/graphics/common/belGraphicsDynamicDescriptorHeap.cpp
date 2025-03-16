/*!
 * @file   belGraphicsDynamicDescriptorHeap.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
bool DynamicDescriptorHeap::initialize(uint32_t num)
{
    // グローバルで大きなデスクリプターヒープを作る
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = num * 2; // ダブルバッファ
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mpDescriptorHeap))))
        {
            return false;
        }
    }
    mDescriptorSize = num;

    return true;
}
//-----------------------------------------------------------------------------
void DynamicDescriptorHeap::swapBuffer()
{
    mBufferIndex = 1 - mBufferIndex;
    mDescriptorIndex.store(0);
}
//-----------------------------------------------------------------------------
// allocate
//-----------------------------------------------------------------------------
DynamicDescriptorHandle DynamicDescriptorHeap::allocate(uint32_t num)
{
    // インデックス移動
    uint32_t index = mDescriptorIndex.fetch_add(num);
    if ((index + num) >= mDescriptorSize)
    {
        // 上限に達した
        return DynamicDescriptorHandle();
    }

    // オフセット計算
    uint32_t offset      = mDescriptorSize * mBufferIndex + index;
    UINT     offset_size = GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * offset;

    // ハンドル取得
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    cpu_handle.ptr += offset_size;

    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = mpDescriptorHeap->GetGPUDescriptorHandleForHeapStart();
    gpu_handle.ptr += offset_size;

    // ハンドルを作成して返す
    DynamicDescriptorHandle handle;
    handle.setProperty(cpu_handle, gpu_handle);
    return handle;
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void DynamicDescriptorHeap::setDescriptorHeap(DynamicDescriptorHandle& handle, CommandContext& command) const
{
    ID3D12DescriptorHeap* p_heaps[] = { mpDescriptorHeap.Get() };
    command.getCommandList().SetDescriptorHeaps(1, p_heaps);
    command.getCommandList().SetGraphicsRootDescriptorTable(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, handle.getGpuHandle());
}
//-----------------------------------------------------------------------------
}