/*!
 * @file   belCommandList.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belCommandList.h"
#include "gfx/core/belRenderTarget.h"
#include "gfx/belGraphics.h"
#include "math/belColor.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool CommandList::initialize(D3D12_COMMAND_LIST_TYPE type)
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_allocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_command_list;
    if (FAILED(Graphics::GetInstance().getDevice().CreateCommandAllocator(type, IID_PPV_ARGS(&p_allocator))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "コマンドアロケーターの生成に失敗しました");
        return false;
    }
    if (FAILED(Graphics::GetInstance().getDevice().CreateCommandList(1, type, p_allocator.Get(), nullptr, IID_PPV_ARGS(&p_command_list))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "コマンドリストの生成に失敗しました");
        return false;
    }
    p_command_list->Close();

    mpAllocator = std::move(p_allocator);
    mpCommandList = std::move(p_command_list);

    return true;
}
//-----------------------------------------------------------------------------
void CommandList::begin()
{
    mpAllocator->Reset();
    mpCommandList->Reset(mpAllocator.Get(), nullptr);
}
//-----------------------------------------------------------------------------
void CommandList::end()
{
    mpCommandList->Close();
}
//-----------------------------------------------------------------------------
// clear
//-----------------------------------------------------------------------------
void CommandList::clearColor(RenderTarget& render_target, const math::Color& color)
{
    FLOAT v[4] = { color.r(), color.g(), color.b(), color.a() };
    D3D12_CPU_DESCRIPTOR_HANDLE handle = render_target.getDescriptorHeap().GetCPUDescriptorHandleForHeapStart();
    mpCommandList->ClearRenderTargetView(handle, v, 0, nullptr);
}
//-----------------------------------------------------------------------------
// resource barrier
//-----------------------------------------------------------------------------
void CommandList::resourceBarrierTransition(ID3D12Resource* p_resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
    D3D12_RESOURCE_BARRIER desc = {};
    desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    desc.Transition.pResource = p_resource;
    desc.Transition.StateBefore = before;
    desc.Transition.StateAfter = after;
    mpCommandList->ResourceBarrier(1, &desc);
}
//-----------------------------------------------------------------------------
}
