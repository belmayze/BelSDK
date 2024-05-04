/*!
 * @file   main.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "base/belApplication.h"
#include "test/testExecutor.h"

/*!
 * エントリーポイント
 */
int belMain(int argc, const char** argv)
{
    // アプリケーション作成
    bel::Application& application = bel::Application::GetInstance();

    // テスト実行
    app::test::Executor::GetInstance().execute();

    // 初期化
    {
        bel::Application::InitializeArg init_arg;
        init_arg.width        = 1280;
        init_arg.height       = 720;
        init_arg.title        = "BelTest";
        init_arg.window_class = "belmayze";

        if (!application.initialize(init_arg)) { return -1; }
    }

    // ゲームループ開始
    return application.enterLoop();
}
