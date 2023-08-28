/*!
 * @file   belCommandQueue.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belCommandList.h"
#include "gfx/core/belCommandQueue.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool CommandQueue::initialize(D3D12_COMMAND_LIST_TYPE type)
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> p_cmd_queue;
    if (FAILED(Graphics::GetInstance().getDevice().CreateCommandQueue(&desc, IID_PPV_ARGS(&p_cmd_queue))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "コマンドキューの生成に失敗しました");
        return false;
    }
    mpCommandQueue = std::move(p_cmd_queue);

    return true;
}
//-----------------------------------------------------------------------------
void CommandQueue::executeCommand(const CommandList& command)
{
    ID3D12GraphicsCommandList& command_list = command.getCommandList();
    ID3D12CommandList* p = &command_list;
    mpCommandQueue->ExecuteCommandLists(1, &p);
}
//-----------------------------------------------------------------------------
// fence
//-----------------------------------------------------------------------------
void CommandQueue::signal(ID3D12Fence* p_fence, UINT64 value)
{
    mpCommandQueue->Signal(p_fence, value);
}
//-----------------------------------------------------------------------------
}
