/*!
 * @file   testApplication.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceLoader.h"
// app
#include "test/testApplication.h"

namespace app::test {
//-----------------------------------------------------------------------------
void Application::initialize()
{
    // メッシュ保持
    {
        mMeshHolder.initialize();
    }

    // パイプライン生成
    {
        mResShaderResource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/Sample.bsh");

        bel::gfx::Pipeline::InitializeArg init_arg;
        init_arg.mNumRenderTarget        = 1;
        init_arg.mRenderTargetFormats[0] = bel::gfx::TextureFormat::cR11G11B10_Float;
        init_arg.mDepthStencilFormat     = bel::gfx::TextureFormat::cD32_Float;
        init_arg.mNumConstantBuffer      = 1;

        init_arg.mDepthConfig.mDepthEnable = true;
        init_arg.mDepthConfig.mDepthWrite  = true;

        mPipeline.initialize(init_arg, mResShaderResource);
    }
    {
        mResToneMappingShaderResource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/ToneMapping.bsh");

        bel::gfx::Pipeline::InitializeArg init_arg;
        init_arg.mNumRenderTarget        = 1;
        init_arg.mRenderTargetFormats[0] = bel::gfx::TextureFormat::cR8G8B8A8_sRGB;
        init_arg.mNumTexture             = 1;
        init_arg.mNumConstantBuffer      = 1;

        mToneMappingPipeline.initialize(init_arg, mResToneMappingShaderResource);
    }

    // スクリーン用メッシュ生成
    {
        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.mIndexBufferSize = sizeof(uint16_t) * 3;
        mScreenMesh.initialize(init_arg);
    }
    
    // 定数バッファー（モデル）
    {
        bel::gfx::ConstantBuffer::InitializeArg init_arg;
        init_arg.mNumBuffer  = 2;
        init_arg.mBufferSize = sizeof(ModelCB);
        mModelCB.initialize(init_arg);

        ModelCB cb;
        cb.mWorldMatrix.makeIdentity();
        cb.mViewProjectionMatrix.makeIdentity();
        mModelCB.copyStruct(cb);
    }

    // 定数バッファー（トーンマッピング）
    {
        bel::gfx::ConstantBuffer::InitializeArg init_arg;
        init_arg.mNumBuffer  = 2;
        init_arg.mBufferSize = sizeof(bel::Color);
        mToneMappingCB.initialize(init_arg);

        bel::Color color = bel::Color::cRed();
        mToneMappingCB.copyStruct(color);
    }

    // 出力バッファー生成
    {
        uint32_t render_width  = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getWidth();
        uint32_t render_height = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getHeight();

        // カラーバッファー
        bel::gfx::Texture::InitializeArg init_arg;
        init_arg.mWidth     = render_width;
        init_arg.mHeight    = render_height;
        init_arg.mDimension = bel::gfx::TextureDimension::c2D;
        init_arg.mFormat    = bel::gfx::TextureFormat::cR11G11B10_Float;
        mColorTexture.initialize(init_arg);

        // デプスバッファー
        init_arg.mFormat = bel::gfx::TextureFormat::cD32_Float;
        mDepthTexture.initialize(init_arg);

        // レンダーターゲット
        mRenderTarget.initialize(mColorTexture);
        mDepthStencil.initialize(mDepthTexture);

        // レンダーバッファー
        mRenderBuffer.setRenderTarget(0, mRenderTarget);
        mRenderBuffer.setDepthStencil(mDepthStencil);
        mRenderBuffer.setResolution(render_width, render_height);
    }
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
void Application::onCalc()
{
    // カメラ更新
    {
        ModelCB cb;

        // ワールド行列は固定
        cb.mWorldMatrix.makeIdentity();

        // ビュープロジェクション行列
        {
            static float sFrameCount = 0.f;
            sFrameCount += 1.f;
            if (sFrameCount >= 360.f) { sFrameCount = 0.f; }

            // カメラ行列
            bel::Matrix34 view_matrix;
            view_matrix.makeLookAtRH(
                bel::Vector3(std::sin(bel::Radian(bel::Degree(sFrameCount))) * 2.f, 1.f, std::cos(bel::Radian(bel::Degree(sFrameCount))) * 2.f),
                bel::Vector3(0.f, 0.f, 0.f),
                bel::Vector3(0.f, 1.f, 0.f)
            );

            // プロジェクション行列
            uint32_t render_width  = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getWidth();
            uint32_t render_height = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getHeight();

            bel::Matrix44 proj_matrix;
            proj_matrix.makePerspectiveProjectionRH(
                bel::Radian(bel::Degree(45.f)),
                static_cast<float>(render_width) / static_cast<float>(render_height),
                1.f, 1000.f
            );
            cb.mViewProjectionMatrix.setMul(proj_matrix, view_matrix);
        }

        mModelCB.swapBuffer();
        mModelCB.copyStruct(cb);
    }

    // 定数バッファーのカラー更新
    {
        static float sFrameCount = 0.f;
        sFrameCount += 0.01f;
        if (sFrameCount >= 3.f) { sFrameCount = 0.f; }

        bel::Color color = bel::Color::cWhite();
        if (sFrameCount < 1.f)
        {
            // R -> G
            float range = bel::Math::EaseInOutSine(sFrameCount);
            color.r() = 1.f - range;
            color.g() = range;
            color.b() = 0.f;
        }
        else if (sFrameCount < 2.f)
        {
            // G -> B
            float range = bel::Math::EaseInOutSine(sFrameCount - 1.f);
            color.r() = 0.f;
            color.g() = 1.f - range;
            color.b() = range;
        }
        else if (sFrameCount < 3.f)
        {
            // B -> R
            float range = bel::Math::EaseInOutSine(sFrameCount - 2.f);
            color.r() = range;
            color.g() = 0.f;
            color.b() = 1.f - range;
        }

        // sRGB -> linear
        color = color.convertToLinearGamut();

        mToneMappingCB.swapBuffer();
        mToneMappingCB.copyStruct(color);
    }
}
//-----------------------------------------------------------------------------
void Application::onMakeCommand(bel::gfx::CommandContext& command) const
{
    // レンダーバッファー切り替え
    mColorTexture.barrierTransition(command, bel::gfx::ResourceState::cRenderTarget);
    mDepthTexture.barrierTransition(command, bel::gfx::ResourceState::cDepthWrite);
    mRenderBuffer.bind(command);

    // クリア
    mRenderBuffer.clear(command, bel::Color::cBlack(), 1.f, 0, { bel::gfx::EClearType::cColor, bel::gfx::EClearType::cDepth });

    // 三角形描画
    mPipeline.activateConstantBuffer(0, mModelCB);
    mPipeline.setPipeline(command);
    mMeshHolder.getMesh(bel::gfx::dev::MeshHolder::Type::cCube).drawIndexedInstanced(command);

    // バリア
    mColorTexture.barrierTransition(command, bel::gfx::ResourceState::cGenericRead);

    // オフスクリーンレンダリングのテクスチャーを設定して描画
    bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().bind(command);
    mToneMappingPipeline.activateTexture(0, mColorTexture);
    mToneMappingPipeline.activateConstantBuffer(0, mToneMappingCB);
    mToneMappingPipeline.setPipeline(command);
    mScreenMesh.drawInstanced(command);
}
//-----------------------------------------------------------------------------
}
