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
void RenderBuffer::bind(CommandContext& command) const
{
    {
        // RenderTargets
        uint32_t num_targets = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handles[cMaxRenderTargets];
        for (const RenderTarget* p_target : mpRenderTargets)
        {
            // nullptr が見つかったら終了
            if (!p_target) { break; }

            rtv_handles[num_targets] = p_target->getDescriptorHandle();
            ++num_targets;
        }

        // DepthStencil
        D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle;
        if (mpDepthStencil) { dsv_handle = mpDepthStencil->getDescriptorHandle(); }

        if (num_targets != 0 || mpDepthStencil)
        {
            command.getCommandList().OMSetRenderTargets(
                num_targets,
                num_targets != 0 ? rtv_handles : nullptr,
                FALSE,
                mpDepthStencil ? &dsv_handle : nullptr
            );
        }
    }
}
//-----------------------------------------------------------------------------
void RenderBuffer::clear(CommandContext& command, const Color& color, float depth, uint8_t stencil, EnumFlags<EClearType> clear_flags) const
{
    if (clear_flags.test(EClearType::cColor))
    {
        for (const RenderTarget* p_target : mpRenderTargets)
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