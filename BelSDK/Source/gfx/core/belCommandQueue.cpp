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
CommandQueue::CommandQueue() {}
//-----------------------------------------------------------------------------
CommandQueue::~CommandQueue()
{
    // コマンド実行完了用フェンスを入れる
    mpFence->Signal(0);
    signal(mpFence.Get(), 1);

    // 実行待ち
    HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!handle)
    {
        BEL_ERROR_WINDOW("GraphicsError", "ハンドルの生成に失敗しました");
        return;
    }
    mpFence->SetEventOnCompletion(1, handle);
    WaitForSingleObject(handle, INFINITE);
    CloseHandle(handle);
}
//-----------------------------------------------------------------------------
bool CommandQueue::initialize(D3D12_COMMAND_LIST_TYPE type)
{
    // コマンドキュー
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = type;

    Microsoft::WRL::ComPtr<ID3D12CommandQueue> p_cmd_queue;
    if (FAILED(Graphics::GetInstance().getDevice().CreateCommandQueue(&desc, IID_PPV_ARGS(&p_cmd_queue))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "コマンドキューの生成に失敗しました");
        return false;
    }
    mpCommandQueue = std::move(p_cmd_queue);

    // キュー削除時の実行完了待ち用フェンス
    Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
    if (FAILED(Graphics::GetInstance().getDevice().CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "フェンスの生成に失敗しました");
        return false;
    }
    mpFence = std::move(p_fence);

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
