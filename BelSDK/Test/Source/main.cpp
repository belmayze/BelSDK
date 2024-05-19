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

    // 引数に -content の指定があったらそのパスにする
    const char* p_content = nullptr;
    for (int i_arg = 0; i_arg < argc; ++i_arg)
    {
        if (strcmp(argv[i_arg], "-content") == 0)
        {
            int next_arg = i_arg + 1;
            if (next_arg < argc)
            {
                p_content = argv[next_arg];
            }
        }
    }

    // 初期化
    {
        bel::Application::InitializeArg init_arg;
        init_arg.width        = 1280;
        init_arg.height       = 720;
        init_arg.title        = "BelTest";
        init_arg.window_class = "belmayze";
        init_arg.content_root = p_content;

        if (!application.initialize(init_arg)) { return -1; }
    }

    // ゲームループ開始
    return application.enterLoop();
}
