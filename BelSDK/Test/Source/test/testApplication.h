/*!
 * @file   testApplication.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "base/belIApplicationCallback.h"
#include "common/belIModule.h"
#include "debug/text/belDebugTextRender.h"
#include "graphics/common/belGraphicsConstantBuffer.h"
#include "graphics/common/belGraphicsDepthStencil.h"
#include "graphics/common/belGraphicsMesh.h"
#include "graphics/common/belGraphicsPipeline.h"
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
#include "resource/belResourceShaderResource.h"
#include "resource/belResourceTexture.h"

namespace app::test {
//-----------------------------------------------------------------------------
class Application : public bel::IApplicationCallback
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     */
    void initialize();

    //-------------------------------------------------------------------------
    // callback
    //-------------------------------------------------------------------------
public:
    /*!
     * 描画コマンド生成前の計算タイミングで呼ばれます
     */
    virtual void onCalc() override;

    /*!
     * 描画コマンドを生成するタイミングで呼ばれます
     * @param[in] command コマンド
     */
    virtual void onMakeCommand(bel::gfx::CommandContext& command) const override;

    //-------------------------------------------------------------------------
private:
    //! 定数バッファー構造体
    struct ModelCB
    {
        bel::Matrix34 world_matrix;
        bel::Matrix44 view_projection_matrix;
    };
    //! トーンマッピング構造体
    struct ToneMappingCB
    {
        uint32_t display_mapping_type;
        uint32_t tone_mapping_type;
        float    hdr_paper_white;
        float padding_;
    };

    //! モジュール
    enum ModuleType
    {
        cModuleTypeController,
        cModuleTypeNum
    };

    //! レイヤー
    struct Layer
    {
        bel::gfx::Texture      color_texture;
        bel::gfx::Texture      depth_texture;
        bel::gfx::RenderTarget render_target;
        bel::gfx::DepthStencil depth_stencil;
        bel::gfx::RenderBuffer render_buffer;
    };

    //-------------------------------------------------------------------------
private:
    bel::gfx::Pipeline       mPipeline;
    bel::gfx::Pipeline       mToneMappingPipeline;
    bel::gfx::Pipeline       mUIComposePipeline;

    bel::gfx::ConstantBuffer mCubeModelCB;
    bel::gfx::ConstantBuffer mSphereModelCB;
    bel::gfx::ConstantBuffer mToneMappingCB;

    Layer m3DLayer;
    Layer mUILayer;

    bel::debug::TextRender mTextRender;

    std::array<std::unique_ptr<bel::IModule>, cModuleTypeNum> mModules;
};
//-----------------------------------------------------------------------------
}
