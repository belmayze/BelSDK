/*!
 * @file   belMain.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <clocale>
// bel
#include "base/belApplication.h"
#include "belMain.h"
// bel/singleton
#include "base/belApplication.h"
#include "debug/performance/belPerfTime.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceLoader.h"

namespace {

//-----------------------------------------------------------------------------
// メインを呼び出す前の事前処理
//-----------------------------------------------------------------------------
static void PreMain()
{
    // ロケーションを日本語に
    std::setlocale(LC_ALL, "");

    // 使用するインスタンスはここで初期化
    bel::Application::GetInstance();
    bel::debug::PerfTime::GetInstance().initialize();
    bel::GraphicsEngine::GetInstance();
    bel::res::Loader::GetInstance();
}

}

//-----------------------------------------------------------------------------
/*!
 * エントリーポイント
 * Console
 */
int main(int argc, const char** argv)
{
    PreMain();
    return belMain(argc, argv);
}

/*!
 * エントリーポイント
 * Windows
 */
int APIENTRY WinMain(
    _In_     HINSTANCE instance,
    _In_opt_ HINSTANCE prev_instance,
    _In_     LPSTR     p_cmd_line,
    _In_     int       cmd_show
)
{
    PreMain();
    return belMain(__argc, const_cast<const char**>(__argv));
}
