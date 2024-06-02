/*!
 * @file   testApplication.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceLoader.h"
// app
#include "test/testApplication.h"

namespace app::test {
//-----------------------------------------------------------------------------
void Application::initialize()
{
    // リソース読み込み
    mResShaderArchive = bel::res::Loader::GetInstance().loadSyncAs<bel::res::ShaderArchive>("Shader/Sample.bshar");

    // パイプライン生成
    mPipeline.initialize(bel::gfx::Pipeline::InitializeArg(), mResShaderArchive);

    // メッシュ生成
    {
        std::array<float, (3 + 3) * 3> vertices = {
            // position      , normal
             0.0f,  0.5f, 0.f, 0.f, 0.f, 1.f,
            -0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f,
             0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f
        };
        std::array<uint16_t, 3> indices = { 0, 1, 2 };

        bel::gfx::Mesh::InitializeArg init_arg;
        init_arg.mpVertexBuffer    = vertices.data();
        init_arg.mVertexBufferSize = sizeof(float) * vertices.size();
        init_arg.mpIndexBuffer     = indices.data();
        init_arg.mIndexBufferSize  = sizeof(uint16_t) * indices.size();
        init_arg.mVertexStride     = sizeof(float) * (3 + 3);
        mMesh.initialize(init_arg);
    }
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
void Application::onCalc()
{

}
//-----------------------------------------------------------------------------
void Application::onMakeCommand(bel::gfx::CommandContext& command) const
{
    mPipeline.setPipeline(command);
    mMesh.drawIndexedInstanced(command);
}
//-----------------------------------------------------------------------------
}
