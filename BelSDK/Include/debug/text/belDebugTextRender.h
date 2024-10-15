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

namespace bel::gfx { class CommandContext; }

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
     * @param[in] max_length    最大文字数
     * @param[in] output_format 出力先フォーマット
     */
    void initialize(uint32_t max_length, gfx::TextureFormat output_format);

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
        InstanceCB instances[1024];
    };

    //-------------------------------------------------------------------------
private:
    gfx::Pipeline               mPipeline;
    mutable gfx::ConstantBuffer mConstantBuffer;
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
