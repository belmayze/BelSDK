/*!
 * @file   belGraphicsViewport.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class RenderBuffer; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// ビューポート
//-----------------------------------------------------------------------------
class Viewport
{
    //-------------------------------------------------------------------------
    // コンストラクター
    //-------------------------------------------------------------------------
public:
    //! 引数なし初期化
    Viewport() {}

    //! レンダーバッファーを使って初期化
    Viewport(const RenderBuffer& render_buffer);

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * ビューポートを適用します
     * @param[in] command コマンド
     */
    void setCommand(CommandContext& command) const;

    //-------------------------------------------------------------------------
private:
    float mX        = 0.f;
    float mY        = 0.f;
    float mWidth    = 0.f;
    float mHeight   = 0.f;
    float mMinDepth = 0.f;
    float mMaxDepth = 1.f;
};
//-----------------------------------------------------------------------------
}
