/*!
 * @file   belGraphicsRenderBuffer.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class RenderTarget; }
namespace bel::gfx { class DepthStencil; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// レンダーバッファー
//-----------------------------------------------------------------------------
class RenderBuffer
{
    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * 任意の色でクリアします
     * @param[in] command     コマンド
     * @param[in] color       クリアカラー
     * @param[in] depth       デプス
     * @param[in] stencil     ステンシル
     * @param[in] clear_flags クリアフラグ
     */
    void clear(CommandContext& command, const Color& color, float depth, uint8_t stencil, EnumFlags<EClearType> clear_flags) const;

    //-------------------------------------------------------------------------
private:
    std::array<RenderTarget*, cMaxRenderTargets> mpRenderTargets = { nullptr };
    DepthStencil*                                mpDepthStencil = nullptr;
};
//-----------------------------------------------------------------------------
}
