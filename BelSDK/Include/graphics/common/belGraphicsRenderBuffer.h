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
    // setter
    //-------------------------------------------------------------------------
public:
    /*!
     * レンダーターゲットを設定する
     * @param[in] index  インデックス
     * @param[in] target ターゲット
     */
    void setRenderTarget(uint32_t index, const RenderTarget& target) { mpRenderTargets[index] = &target; }

    /*!
     * デプスステンシルを設定する
     * @param[in] depth_stencil デプスステンシル
     */
    void setDepthStencil(const DepthStencil& depth_stencil) { mpDepthStencil = &depth_stencil; }

    /*!
     * レンダーターゲットをクリアします
     */
    void clearRenderTargets() { mpRenderTargets.fill(nullptr); }

    /*!
     * デプスステンシルをクリアします
     */
    void clearDepthStencil() { mpDepthStencil = nullptr; }

    /*!
     * バッファー解像度を設定する
     */
    void setResolution(uint32_t width, uint32_t height) { mWidth = width; mHeight = height; }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 横幅
    uint32_t getWidth() const { return mWidth; }
    //! 高さ
    uint32_t getHeight() const { return mHeight; }

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * 使えるようにターゲットをバインドします
     * @param[in] command コマンド
     */
    void bind(CommandContext& command) const;

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
    std::array<const RenderTarget*, cMaxRenderTargets> mpRenderTargets = { nullptr };
    const DepthStencil*                                mpDepthStencil = nullptr;
    uint32_t mWidth  = 0;
    uint32_t mHeight = 0;
};
//-----------------------------------------------------------------------------
}
