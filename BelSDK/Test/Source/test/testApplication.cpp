/*!
 * @file   testApplication.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/common/belGraphicsDynamicTextureResource.h"
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
        init_arg.buffer_size = sizeof(ToneMappingCB);
        mToneMappingCB.initialize(init_arg);

        ToneMappingCB cb;
        cb.display_mapping_type = bel::GraphicsEngine::GetInstance().isHDREnabled() ? 1 : 0;
        cb.tone_mapping_type = 0;
        cb.hdr_paper_white = 0;
        mToneMappingCB.copyStruct(cb);
    }

    // 出力バッファー生成
    {
        uint32_t render_width  = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getWidth();
        uint32_t render_height = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getHeight();

        // 3Dレイヤー
        {
            // レンダーターゲット
            m3DLayer.render_target.initialize();
            m3DLayer.depth_stencil.initialize();

            // レンダーバッファー
            m3DLayer.render_buffer.setRenderTarget(0, m3DLayer.render_target);
            m3DLayer.render_buffer.setDepthStencil(m3DLayer.depth_stencil);
            m3DLayer.render_buffer.setResolution(render_width, render_height);
        }

        // UIレイヤー
        {
            // レンダーターゲット
            mUILayer.render_target.initialize();

            // レンダーバッファー
            mUILayer.render_buffer.setRenderTarget(0, mUILayer.render_target);
            mUILayer.render_buffer.setResolution(render_width, render_height);
        }
    }

    // パイプライン生成
    {
        auto resource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/Sample.bsh");

        bel::gfx::Pipeline::InitializeArg init_arg;
        init_arg.num_render_target        = 1;
        init_arg.render_target_formats[0] = bel::gfx::TextureFormat::cR16G16B16A16_Float;
        init_arg.depth_stencil_format     = bel::gfx::TextureFormat::cD32_Float;
        init_arg.num_constant_buffer      = 1;

        init_arg.depth_config.depth_enable = true;
        init_arg.depth_config.depth_write  = true;

        mPipeline.initialize(init_arg, resource);
    }
    {
        auto resource = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderResource>("Shader/ToneMapping.bsh");

        {
            bel::gfx::Pipeline::InitializeArg init_arg;
            init_arg.num_render_target        = 1;
            init_arg.render_target_formats[0] = bel::GraphicsEngine::GetInstance().getDefaultRenderTarget().getTexture().getFormat();
            init_arg.num_texture              = 1;
            init_arg.num_constant_buffer      = 1;

            mToneMappingPipeline.initialize(init_arg, resource);
        }
        {
            bel::gfx::Pipeline::InitializeArg init_arg;
            init_arg.num_render_target        = 1;
            init_arg.render_target_formats[0] = bel::GraphicsEngine::GetInstance().getDefaultRenderTarget().getTexture().getFormat();
            init_arg.blend_configs[0].blend_enable = true;
            init_arg.num_texture              = 1;
            init_arg.num_constant_buffer      = 1;

            mUIComposePipeline.initialize(init_arg, resource);
        }
    }

    // 計測初期化
    bel::debug::PerfTime::GetInstance().initialize(mUILayer.color_texture.getFormat());

    // デバッグ文字列
    mTextRender.initialize(1024, mUILayer.color_texture.getFormat());
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
        ToneMappingCB cb;
        cb.display_mapping_type = bel::GraphicsEngine::GetInstance().isHDREnabled() ? 1 : 0;
        cb.tone_mapping_type = 0;
        cb.hdr_paper_white = bel::GraphicsEngine::GetInstance().getHDRPaperWhite();

        mToneMappingCB.swapBuffer();
        mToneMappingCB.copyStruct(cb);
    }
}
//-----------------------------------------------------------------------------
void Application::onMakeCommand(bel::gfx::CommandContext& command) const
{
    // クラス
    bel::gfx::DynamicDescriptorHeap& descriptor_heap = bel::gfx::DynamicDescriptorHeap::GetInstance();
    bel::gfx::dev::MeshHolder& mesh_holder = bel::gfx::dev::MeshHolder::GetInstance();

    // 3Dレイヤー
    {
        // テクスチャー確保
        {
            bel::gfx::DynamicTextureResource::AllocateArg alloc_arg;
            alloc_arg.width = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getWidth();
            alloc_arg.height = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getHeight();
            alloc_arg.format = bel::gfx::TextureFormat::cR16G16B16A16_Float;
            alloc_arg.resource_flags = bel::gfx::ResourceFlag::cRenderTarget;
            m3DLayer.color_texture = bel::gfx::DynamicTextureResource::GetInstance().allocate(alloc_arg);

            alloc_arg.format = bel::gfx::TextureFormat::cD32_Float;
            alloc_arg.resource_flags = bel::gfx::ResourceFlag::cDepthStencil;
            m3DLayer.depth_texture = bel::gfx::DynamicTextureResource::GetInstance().allocate(alloc_arg);

            m3DLayer.render_target.updateTexture(m3DLayer.color_texture);
            m3DLayer.depth_stencil.updateTexture(m3DLayer.depth_texture);
        }

        // レンダーバッファー切り替え
        m3DLayer.color_texture.barrierTransition(command, bel::gfx::ResourceState::cRenderTarget);
        m3DLayer.depth_texture.barrierTransition(command, bel::gfx::ResourceState::cDepthWrite);
        m3DLayer.render_buffer.bind(command);

        // クリア
        m3DLayer.render_buffer.clear(command, bel::Color::cBlack(), 1.f, 0, { bel::gfx::EClearType::cColor, bel::gfx::EClearType::cDepth });

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
        m3DLayer.color_texture.barrierTransition(command, bel::gfx::ResourceState::cGenericRead);

        // オフスクリーンレンダリングのテクスチャーを設定して描画
        {
            bel::gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mToneMappingPipeline.getNumDescriptor());

            bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().bind(command);
            mToneMappingPipeline.activateTexture(handle, 0, m3DLayer.color_texture);
            mToneMappingPipeline.activateConstantBuffer(handle, 0, mToneMappingCB);
            mToneMappingPipeline.setPipeline(command);
            descriptor_heap.setDescriptorHeap(handle, command);
            mesh_holder.getMesh(bel::gfx::dev::MeshHolder::Type::cQuadTriangle).drawInstanced(command);
        }

        // ここでテクスチャー解放
        bel::gfx::DynamicTextureResource::GetInstance().free(std::move(m3DLayer.color_texture));
        bel::gfx::DynamicTextureResource::GetInstance().free(std::move(m3DLayer.depth_texture));
    }

    // UIレイヤー
    {
        // テクスチャー確保
        {
            bel::gfx::DynamicTextureResource::AllocateArg alloc_arg;
            alloc_arg.width = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getWidth();
            alloc_arg.height = bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().getHeight();
            alloc_arg.format = bel::gfx::TextureFormat::cR8G8B8A8_uNorm;
            alloc_arg.optimized_clear_color = bel::Color::cZero();
            alloc_arg.resource_flags = bel::gfx::ResourceFlag::cRenderTarget;
            mUILayer.color_texture = bel::gfx::DynamicTextureResource::GetInstance().allocate(alloc_arg);

            mUILayer.render_target.updateTexture(mUILayer.color_texture);
        }

        // レンダーバッファー切り替え
        mUILayer.color_texture.barrierTransition(command, bel::gfx::ResourceState::cRenderTarget);
        mUILayer.render_buffer.bind(command);

        // クリア
        mUILayer.render_buffer.clear(command, bel::Color::cZero(), 1.f, 0, {bel::gfx::EClearType::cColor});

        //// 文字列描画
        mTextRender.draw(command);

        //// デバッグ描画
        bel::debug::PerfTime::GetInstance().drawDebugText(command);

        // バリア
        mUILayer.color_texture.barrierTransition(command, bel::gfx::ResourceState::cGenericRead);

        // UI描画を合成
        {
            bel::gfx::DynamicDescriptorHandle handle = descriptor_heap.allocate(mUIComposePipeline.getNumDescriptor());

            bel::GraphicsEngine::GetInstance().getDefaultRenderBuffer().bind(command);
            mUIComposePipeline.activateTexture(handle, 0, mUILayer.color_texture);
            mUIComposePipeline.activateConstantBuffer(handle, 0, mToneMappingCB);
            mUIComposePipeline.setPipeline(command);
            descriptor_heap.setDescriptorHeap(handle, command);
            mesh_holder.getMesh(bel::gfx::dev::MeshHolder::Type::cQuadTriangle).drawInstanced(command);
        }

        // ここでテクスチャー解放
        bel::gfx::DynamicTextureResource::GetInstance().free(std::move(mUILayer.color_texture));
    }
}
//-----------------------------------------------------------------------------
}
