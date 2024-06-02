/*!
 * @file   belGraphicsRenderTarget.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// レンダーターゲット
//-----------------------------------------------------------------------------
class RenderTarget
{
    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] texture テクスチャー
     */
    bool initialize(const Texture& texture);

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
     * 任意の色でクリアします
     * @param[in] command コマンド
     * @param[in] color   クリアカラー
     */
    void clear(CommandContext& command, const Color& color) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    const Texture* mpTexture = nullptr;
};
//-----------------------------------------------------------------------------
}
