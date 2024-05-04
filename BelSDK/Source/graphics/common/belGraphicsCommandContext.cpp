/*!
 * @file   belGraphicsCommandContext.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsCommandContext.h"
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/common/belGraphicsCommandQueue.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
void CommandContext::executeCommand(CommandQueue& command_queue)
{
    ID3D12CommandList* p_command_lists[1] = { &mCommandList.getCommandList() };
    command_queue.getCommandQueue().ExecuteCommandLists(1, p_command_lists);
}
//-----------------------------------------------------------------------------
}