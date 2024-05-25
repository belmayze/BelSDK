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
// getter
//-----------------------------------------------------------------------------
ID3D12GraphicsCommandList& CommandContext::getCommandList()
{
    return mCommandList.getCommandList();
}
//-----------------------------------------------------------------------------
}