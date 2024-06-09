/*!
 * @file   belGraphicsViewport.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/common/belGraphicsViewport.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// コンストラクター
//-----------------------------------------------------------------------------
Viewport::Viewport(const RenderBuffer& render_buffer)
    : mWidth(static_cast<float>(render_buffer.getWidth()))
    , mHeight(static_cast<float>(render_buffer.getHeight()))
{}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Viewport::setCommand(CommandContext& command) const
{
    // ビューポート
    D3D12_VIEWPORT vp;
    vp.TopLeftX = mX;
    vp.TopLeftY = mY;
    vp.Width    = mWidth;
    vp.Height   = mHeight;
    vp.MinDepth = mMinDepth;
    vp.MaxDepth = mMaxDepth;
    command.getCommandList().RSSetViewports(1, &vp);

    // シザーも同じ領域にする
    D3D12_RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = static_cast<LONG>(mWidth);
    rect.bottom = static_cast<LONG>(mHeight);
    command.getCommandList().RSSetScissorRects(1, &rect);
}
//-----------------------------------------------------------------------------
}