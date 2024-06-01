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
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
void Application::onCalc()
{

}
//-----------------------------------------------------------------------------
void Application::onMakeCommand(bel::gfx::CommandContext& command_context) const
{

}
//-----------------------------------------------------------------------------
}
