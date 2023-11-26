/*!
 * @file   belPipelineStateContext.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belPipelineStateContext.h"

namespace bel::gfx
{

//-----------------------------------------------------------------------------
// Mask
//-----------------------------------------------------------------------------
void PipelineStateContext::setColorMask(bool r, bool g, bool b, bool a, uint32_t index)
{
    uint8_t mask = 0;
    if (r) { mask |= D3D12_COLOR_WRITE_ENABLE_RED; }
    if (g) { mask |= D3D12_COLOR_WRITE_ENABLE_GREEN; }
    if (b) { mask |= D3D12_COLOR_WRITE_ENABLE_BLUE; }
    if (a) { mask |= D3D12_COLOR_WRITE_ENABLE_ALPHA; }
    mBlendDesc.mRenderTargets[index].mWriteMask = mask;
}
//-----------------------------------------------------------------------------
// Depth
//-----------------------------------------------------------------------------
void PipelineStateContext::setDepthTest(bool enable, bool write)
{
    mDepthStencilDesc.mEnableDepth = enable;
    mDepthStencilDesc.mDepthWrite  = write;
}
//-----------------------------------------------------------------------------
void PipelineStateContext::setDepthFunc(DepthFunc func)
{
    mDepthStencilDesc.mDepthFunc = func;
}
//-----------------------------------------------------------------------------
// Blend
//-----------------------------------------------------------------------------
void PipelineStateContext::setBlendEnable(bool enable, uint32_t index)
{
    mBlendDesc.mRenderTargets[index].mEnable = enable;
}
//-----------------------------------------------------------------------------
void PipelineStateContext::setBlendFunc(
    BlendFactor src_color, BlendFactor dst_color, BlendOp color_op,
    BlendFactor src_alpha, BlendFactor dst_alpha, BlendOp alpha_op,
    uint32_t index
)
{
    mBlendDesc.mRenderTargets[index].mSrcColor = src_color;
    mBlendDesc.mRenderTargets[index].mDstColor = dst_color;
    mBlendDesc.mRenderTargets[index].mColorOp  = color_op;
    mBlendDesc.mRenderTargets[index].mSrcAlpha = src_alpha;
    mBlendDesc.mRenderTargets[index].mDstAlpha = dst_alpha;
    mBlendDesc.mRenderTargets[index].mAlphaOp  = alpha_op;
}
//-----------------------------------------------------------------------------

}
