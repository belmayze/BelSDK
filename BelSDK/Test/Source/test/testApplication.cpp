/*!
 * @file   testApplication.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/dev/belGraphicsDevMeshHolder.h"
#include "graphics/belGraphicsEngine.h"
#include "hid/belController.h"
#include "resource/belResourceLoader.h"
// bel/modules
#include "hid/belHidModule.h"
// app
#include "test/testApplication.h"

namespace app::test {
//-----------------------------------------------------------------------------
void Application::initialize()
{
    // モジュール初期化
    {
        {
            auto p_module = std::make_unique<bel::hid::HidModule>();
            p_module->initialize();
            mModules[cModuleTypeController] = std::move(p_module);
        }
    }

    // 定数バッファー（モデル）
    {
        bel::gfx::ConstantBuffer::InitializeArg init_arg;
        init_arg.num_buffer  = 2;
        init_arg.buffer_size = sizeof(ModelCB);
        mCubeModelCB.initialize(init_arg);
        mSphereModelCB.initialize(init_arg);

        ModelCB cb;
        cb.world_matrix.makeIdentity();
        cb.view_projection_matrix.makeIdentity();
        mCubeModelCB.copyStruct(cb);
        mSphereModelCB.copyStruct(cb);
    }

    // 定数バッファー（トーンマッピング）
    {
        bel::gfx::ConstantBuffer::InitializeArg init_arg;
        init_arg.num_buffer = 2;
        init_arg.buffer_size = sizeof(bel::Color);
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
        init_arg.width     = render_width;
        init_arg.height    = render_height;
        init_arg.dimension = bel::gfx::TextureDimension::c2D;
        init_arg.format    = bel::gfx::TextureFormat::cR16G16B16A16_Float;
        mColorTexture.initialize(init_arg);

        // デプスバッファー
        init_arg.format = bel::gfx::TextureFormat::cD32_Float;
        mDepthTexture.initialize(init_arg);

        // レンダーターゲット
        mRenderTarget.initialize(mColorTexture);
        mDepthStencil.initialize(mDepthTexture);

        // レンダーバッファー
        mRenderBuffer.setRenderTarget(0, mRenderTarget);
        mRenderBuffer.setDepthStencil(mDepthStencil);
        mRenderBuffer.setResolution(render_width, render_height);
    }

    // パイプライン生成
    {
        auto resource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/Sample.bsh");

        bel::gfx::Pipeline::InitializeArg init_arg;
        init_arg.num_render_target        = 1;
        init_arg.render_target_formats[0] = mColorTexture.getFormat();
        init_arg.depth_stencil_format     = mDepthTexture.getFormat();
        init_arg.num_constant_buffer      = 1;

        init_arg.depth_config.depth_enable = true;
        init_arg.depth_config.depth_write  = true;

        mPipeline.initialize(init_arg, resource);
    }
    {
        auto resource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/ToneMapping.bsh");

        bel::gfx::Pipeline::InitializeArg init_arg;
        init_arg.num_render_target        = 1;
        init_arg.render_target_formats[0] = bel::GraphicsEngine::GetInstance().getDefaultRenderTarget().getTexture().getFormat();
        init_arg.num_texture              = 1;
        init_arg.num_constant_buffer      = 1;

        mToneMappingPipeline.initialize(init_arg, resource);
    }

    // デバッグ文字列
    {
        mTextRender.initialize(1024, bel::GraphicsEngine::GetInstance().getDefaultRenderTarget().getTexture().getFormat());
    }
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
void Application::onCalc()
{
    // モジュールの更新
    for (auto& module : mModules)
    {
        module->update();
    }

    // コントローラーの状況を文字列描画
    {
        bel::hid::HidModule& hid_module = *static_cast<bel::hid::HidModule*>(mModules[cModuleTypeController].get());
        const bel::hid::Controller& controller = hid_module.getController(0);

        char text[1024];
        snprintf(
            text, 1024,
            "CTRL[0] [%s] Press   : %s%s%s%s%s%s%s%s | %s %s %s %s %s %s | %s %s\n"
            "             Trigger : %s%s%s%s%s%s%s%s | %s %s %s %s %s %s | %s %s\n"
            "             Release : %s%s%s%s%s%s%s%s | %s %s %s %s %s %s | %s %s\n"
            "             Stick   : LX[%+.2f] LY[%+.2f] | RX[%+.2f] RY[%+.2f]\n"
            "             Trigger : L[%.2f] R[%.2f]",
            controller.isConnected() ? "OK" : "NG",
            controller.isPress(bel::hid::ButtonType::cA) ? "A" : "-",
            controller.isPress(bel::hid::ButtonType::cB) ? "B" : "-",
            controller.isPress(bel::hid::ButtonType::cX) ? "X" : "-",
            controller.isPress(bel::hid::ButtonType::cY) ? "Y" : "-",
            controller.isPress(bel::hid::ButtonType::cDPadUp)    ? "U" : "-",
            controller.isPress(bel::hid::ButtonType::cDPadDown)  ? "D" : "-",
            controller.isPress(bel::hid::ButtonType::cDPadLeft)  ? "L" : "-",
            controller.isPress(bel::hid::ButtonType::cDPadRight) ? "R" : "-",
            controller.isPress(bel::hid::ButtonType::cL1) ? "L1" : " -",
            controller.isPress(bel::hid::ButtonType::cR1) ? "R1" : " -",
            controller.isPress(bel::hid::ButtonType::cL2) ? "L2" : " -",
            controller.isPress(bel::hid::ButtonType::cR2) ? "R2" : " -",
            controller.isPress(bel::hid::ButtonType::cL3) ? "L3" : " -",
            controller.isPress(bel::hid::ButtonType::cR3) ? "R3" : " -",
            controller.isPress(bel::hid::ButtonType::cStart) ? "ST" : " -",
            controller.isPress(bel::hid::ButtonType::cBack)  ? "BK" : " -",

            controller.isTrigger(bel::hid::ButtonType::cA) ? "A" : "-",
            controller.isTrigger(bel::hid::ButtonType::cB) ? "B" : "-",
            controller.isTrigger(bel::hid::ButtonType::cX) ? "X" : "-",
            controller.isTrigger(bel::hid::ButtonType::cY) ? "Y" : "-",
            controller.isTrigger(bel::hid::ButtonType::cDPadUp)    ? "U" : "-",
            controller.isTrigger(bel::hid::ButtonType::cDPadDown)  ? "D" : "-",
            controller.isTrigger(bel::hid::ButtonType::cDPadLeft)  ? "L" : "-",
            controller.isTrigger(bel::hid::ButtonType::cDPadRight) ? "R" : "-",
            controller.isTrigger(bel::hid::ButtonType::cL1) ? "L1" : " -",
            controller.isTrigger(bel::hid::ButtonType::cR1) ? "R1" : " -",
            controller.isTrigger(bel::hid::ButtonType::cL2) ? "L2" : " -",
            controller.isTrigger(bel::hid::ButtonType::cR2) ? "R2" : " -",
            controller.isTrigger(bel::hid::ButtonType::cL3) ? "L3" : " -",
            controller.isTrigger(bel::hid::ButtonType::cR3) ? "R3" : " -",
            controller.isTrigger(bel::hid::ButtonType::cStart) ? "ST" : " -",
            controller.isTrigger(bel::hid::ButtonType::cBack)  ? "BK" : " -",

            controller.isRelease(bel::hid::ButtonType::cA) ? "A" : "-",
            controller.isRelease(bel::hid::ButtonType::cB) ? "B" : "-",
            controller.isRelease(bel::hid::ButtonType::cX) ? "X" : "-",
            controller.isRelease(bel::hid::ButtonType::cY) ? "Y" : "-",
            controller.isRelease(bel::hid::ButtonType::cDPadUp)    ? "U" : "-",
            controller.isRelease(bel::hid::ButtonType::cDPadDown)  ? "D" : "-",
            controller.isRelease(bel::hid::ButtonType::cDPadLeft)  ? "L" : "-",
            controller.isRelease(bel::hid::ButtonType::cDPadRight) ? "R" : "-",
            controller.isRelease(bel::hid::ButtonType::cL1) ? "L1" : " -",
            controller.isRelease(bel::hid::ButtonType::cR1) ? "R1" : " -",
            controller.isRelease(bel::hid::ButtonType::cL2) ? "L2" : " -",
            controller.isRelease(bel::hid::ButtonType::cR2) ? "R2" : " -",
            controller.isRelease(bel::hid::ButtonType::cL3) ? "L3" : " -",
            controller.isRelease(bel::hid::ButtonType::cR3) ? "R3" : " -",
            controller.isRelease(bel::hid::ButtonType::cStart) ? "ST" : " -",
            controller.isRelease(bel::hid::ButtonType::cBack)  ? "BK" : " -",
            
            controller.getLeftStick().x(),
            controller.getLeftStick().y(),
            controller.getRightStick().x(),
            controller.getRightStick().y(),
            controller.getLeftTrigger(),
            controller.getRightTrigger()
            );
        mTextRender.calcText(text, bel::Vector2(0.f, 0.f), 16.f);
    }

    // カメラ更新
    {
        //
        ModelCB cb;
        cb.world_matrix.makeIdentity();

        // ビュープロジェクション行列
        {
            static float sFrameCount = 0.f;
            sFrameCount += 1.f;
            if (sFrameCount >= 360.f) { sFrameCount = 0.f; }

            // カメラ行列
            bel::Matrix34 view_matrix;
            view_matrix.makeLookAtRH(
                bel::Vector3(std::sin(bel::Radian(bel::Degree(sFrameCount))) * 4.f, 1.f, std::cos(bel::Radian(bel::Degree(sFrameCount))) * 4.f),
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
            cb.view_projection_matrix.setMul(proj_matrix, view_matrix);
        }

        // キューブUBO
        mCubeModelCB.swapBuffer();
        cb.world_matrix.makeTranslate(bel::Vector3(0.75f, 0.f, 0.f));
        mCubeModelCB.getStruct<ModelCB>() = cb;

        // スフィアUBO
        mSphereModelCB.swapBuffer();
        cb.world_matrix.makeTranslate(bel::Vector3(-0.75f, 0.f, 0.f));
        mSphereModelCB.getStruct<ModelCB>() = cb;
    }

    // 定数バッファーのカラー更新
    {
        static float sFrameCount = 0.f;
        sFrameCount += 0.01f;
        if (sFrameCount >= 3.f) { sFrameCount = 0.f; }

        bel::Color color = bel::Color::cWhite();
        //if (sFrameCount < 1.f)
        //{
        //    // R -> G
        //    float range = bel::Math::EaseInOutSine(sFrameCount);
        //    color.r() = 1.f - range;
        //    color.g() = range;
        //    color.b() = 0.f;
        //}
        //else if (sFrameCount < 2.f)
        //{
        //    // G -> B
        //    float range = bel::Math::EaseInOutSine(sFrameCount - 1.f);
        //    color.r() = 0.f;
        //    color.g() = 1.f - range;
        //    color.b() = range;
        //}
        //else if (sFrameCount < 3.f)
        //{
        //    // B -> R
        //    float range = bel::Math::EaseInOutSine(sFrameCount - 2.f);
        //    color.r() = range;
        //    color.g() = 0.f;
        //    color.b() = 1.f - range;
        //}

        // sRGB -> linear
        color = color.convertToLinearGamut();

        mToneMappingCB.swapBuffer();
        mToneMappingCB.copyStruct(color);
    }
}
//-----------------------------------------------------------------------------
void Application::onMakeCommand(bel::gfx::CommandContext& command) const
{
    // クラス
    bel::gfx::DynamicDescriptorHeap& descriptor_heap = bel::gfx::DynamicDescriptorHeap::GetInstance();
    bel::gfx::dev::MeshHolder& mesh_holder = bel::gfx::dev::MeshHolder::GetInstance();

    // レンダーバッファー切り替え
    mColorTexture.barrierTransition(command, bel::gfx::ResourceState::cRenderTarget);
    mDepthTexture.barrierTransition(command, bel::gfx::ResourceState::cDepthWrite);
    mRenderBuffer.bind(command);

    // クリア
    mRenderBuffer.clear(command, bel::Color::cBlack(), 1.f, 0, { bel::gfx::EClearType::cColor, bel::gfx::EClearType::cDepth });

    // キューブ描画
    {
        bel::gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mPipeline.getNumDescriptor());

        mPipeline.activateConstantBuffer(handle, 0, mCubeModelCB);
        mPipeline.setPipeline(command);
        descriptor_heap.setDescriptorHeap(handle, command);
        mesh_holder.getMesh(bel::gfx::dev::MeshHolder::Type::cCube).drawIndexedInstanced(command);
    }
    // スフィア描画
    {
        bel::gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mPipeline.getNumDescriptor());
        
        mPipeline.activateConstantBuffer(handle, 0, mSphereModelCB);
        mPipeline.setPipeline(command);
        descriptor_heap.setDescriptorHeap(handle, command);
        mesh_holder.getMesh(bel::gfx::dev::MeshHolder::Type::cSphere).drawIndexedInstanced(command);
    }

    // バリア
    mColorTexture.barrierTransition(command, bel::gfx::ResourceState::cGenericRead);

    // オフスクリーンレンダリングのテクスチャーを設定して描画
    {
        bel::gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mToneMappingPipeline.getNumDescriptor());

        bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().bind(command);
        mToneMappingPipeline.activateTexture(handle, 0, mColorTexture);
        mToneMappingPipeline.activateConstantBuffer(handle, 0, mToneMappingCB);
        mToneMappingPipeline.setPipeline(command);
        descriptor_heap.setDescriptorHeap(handle, command);
        mesh_holder.getMesh(bel::gfx::dev::MeshHolder::Type::cQuadTriangle).drawInstanced(command);
    }

    // 文字列描画
    mTextRender.draw(command);
}
//-----------------------------------------------------------------------------
}
