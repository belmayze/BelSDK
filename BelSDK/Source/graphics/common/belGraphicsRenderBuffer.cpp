/*!
 * @file   belGraphicsRenderBuffer.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDepthStencil.h"
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void RenderBuffer::clear(CommandContext& command, const Color& color, float depth, uint8_t stencil, EnumFlags<EClearType> clear_flags) const
{
    if (clear_flags.test(EClearType::cColor))
    {
        for (RenderTarget* p_target : mpRenderTargets)
        {
            if (p_target) { p_target->clear(command, color); }
        }
    }
    if (clear_flags.testAny(EClearType::cDepth, EClearType::cStencil))
    {
        if (mpDepthStencil) { mpDepthStencil->clear(command, depth, stencil, clear_flags); }
    }
}
//-----------------------------------------------------------------------------
}