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

    //-------------------------------------------------------------------------
private:
    bel::gfx::Pipeline       mPipeline;
    bel::gfx::Pipeline       mToneMappingPipeline;
    bel::gfx::Mesh           mScreenMesh;

    bel::gfx::ConstantBuffer mModelCB;
    bel::gfx::ConstantBuffer mToneMappingCB;

    bel::gfx::Texture      mColorTexture;
    bel::gfx::Texture      mDepthTexture;
    bel::gfx::RenderTarget mRenderTarget;
    bel::gfx::DepthStencil mDepthStencil;
    bel::gfx::RenderBuffer mRenderBuffer;
};
//-----------------------------------------------------------------------------
}
