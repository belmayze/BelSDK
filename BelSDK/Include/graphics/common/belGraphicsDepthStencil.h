/*!
 * @file   belGraphicsDepthStencil.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// デプスステンシル
//-----------------------------------------------------------------------------
class DepthStencil
{
    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     */
    bool initialize();

    /*!
     * テクスチャーを更新
     * @param[in] texture テクスチャー
     */
    void updateTexture(const Texture& texture);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! テクスチャーを取得
    const Texture& getTexture() const { BEL_ASSERT(mpTexture != nullptr); return *mpTexture; }

    //! デスクリプターハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle() const { return mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart(); }

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * 任意の値でクリアします
     * @param[in] command     コマンド
     * @param[in] depth       デプス値
     * @param[in] stencil     ステンシル値
     * @param[in] clear_flags クリアフラグ
     */
    void clear(CommandContext& command, float depth, uint8_t stencil, EnumFlags<EClearType> clear_flags) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    const Texture* mpTexture = nullptr;
};
//-----------------------------------------------------------------------------
}
