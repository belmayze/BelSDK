/*!
 * @file   belGraphicsCommandList.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool CommandList::initialize(CommandType type)
{
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    p_allocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_command_list;

    // 種別決定
    D3D12_COMMAND_LIST_TYPE command_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    switch (type)
    {
    case CommandType::cGraphics:
        command_type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        break;
    case CommandType::cCompute:
        command_type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
        break;
    case CommandType::cCopy:
        command_type = D3D12_COMMAND_LIST_TYPE_COPY;
        break;
    }

    if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommandAllocator(command_type, IID_PPV_ARGS(&p_allocator))))
    {
        BEL_ERROR_LOG("コマンドアロケーターの生成に失敗しました\n");
        return false;
    }
    if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommandList(1, command_type, p_allocator.Get(), nullptr, IID_PPV_ARGS(&p_command_list))))
    {
        BEL_ERROR_LOG("コマンドリストの生成に失敗しました\n");
        return false;
    }
    p_command_list->Close();

    // 保持
    mpAllocator = std::move(p_allocator);
    mpCommandList = std::move(p_command_list);

    return true;
}
//-----------------------------------------------------------------------------
// recording
//-----------------------------------------------------------------------------
void CommandList::begin()
{
    // アロケーターをリセットしてから記録開始
    mpAllocator->Reset();
    mpCommandList->Reset(mpAllocator.Get(), nullptr);
}
//-----------------------------------------------------------------------------
void CommandList::end()
{
    // 記録終了
    mpCommandList->Close();
}
//-----------------------------------------------------------------------------
}