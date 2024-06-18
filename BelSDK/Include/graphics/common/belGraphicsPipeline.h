/*!
 * @file   belGraphicsPipeline.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"

namespace bel::res { class ShaderResource; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// パイプライン
//-----------------------------------------------------------------------------
class Pipeline
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t                                     num_render_target = 0;
        std::array<TextureFormat, cMaxRenderTargets> render_target_formats = {};
        TextureFormat                                depth_stencil_format = TextureFormat::cUnknown;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] arg    初期化引数
     * @param[in] shader シェーダーリソース
     */
    bool initialize(const InitializeArg& arg, const res::ShaderResource& shader);

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * パイプラインを設定します
     * @param[in] command コマンド
     */
    void setPipeline(CommandContext& command) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> mpPipeline;
};
//-----------------------------------------------------------------------------
}
