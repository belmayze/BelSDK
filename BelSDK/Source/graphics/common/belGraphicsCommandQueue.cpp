/*!
 * @file   belGraphicsCommandQueue.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsCommandQueue.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool CommandQueue::initialize(CommandType type)
{
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> p_command_queue;

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

    //
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = command_type;

    if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateCommandQueue(&desc, IID_PPV_ARGS(&p_command_queue))))
    {
        BEL_ERROR_LOG("コマンドキューの生成に失敗しました\n");
        return false;
    }

    // 保持
    mpCommandQueue = std::move(p_command_queue);

    return true;
}
//-----------------------------------------------------------------------------
}