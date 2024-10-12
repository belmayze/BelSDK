/*!
 * @file   belDebugTextRender.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "math/belVector2.h"
#include "graphics/common/belGraphicsConstantBuffer.h"
#include "graphics/common/belGraphicsPipeline.h"
#include "graphics/common/belGraphicsTexture.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
class TextRender
{
    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] max_length 最大文字数
     */
    void initialize(uint32_t max_length);

    //-------------------------------------------------------------------------
    // render
    //-------------------------------------------------------------------------
public:
    /*!
     * 文字列を描画します
     * @param[in] command  コマンド
     * @param[in] text     テキスト
     * @param[in] position 描画座標
     * @param[in] size     フォントサイズ
     */
    void draw(gfx::CommandContext& command, const char* text, const Vector2& position, float size) const;

    //-------------------------------------------------------------------------
private:
    //! 定数バッファー構造体
    struct InstanceCB
    {
        Vector2 position;
        Vector2 uv_offset;
        Vector2 uv_scale;
        Vector2 size;
    };
    struct InstancesCB
    {
        float size;
        float padding_[3];
        InstanceCB instances[1024];
    };

    //-------------------------------------------------------------------------
private:
    gfx::Texture        mTexture; //!< @todo: 本当は共用なので ResourceHolder 的な所に置く
    gfx::Pipeline       mPipeline;
    mutable gfx::ConstantBuffer mConstantBuffer;
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
