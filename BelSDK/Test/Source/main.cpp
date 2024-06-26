﻿/*!
 * @file   main.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "base/belApplication.h"
#include "test/testApplication.h"
#include "test/testExecutor.h"

/*!
 * エントリーポイント
 */
int belMain(int argc, const char** argv)
{
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

    // テストアプリケーション
    app::test::Application application;

    // 初期化
    {
        bel::Application::InitializeArg init_arg;
        init_arg.width             = 1280;
        init_arg.height            = 720;
        init_arg.title             = "BelTest";
        init_arg.window_class_name = "belmayze";
        init_arg.content_root      = p_content;
        init_arg.p_callback        = &application;

        if (!bel::Application::GetInstance().initialize(init_arg)) { return -1; }
    }

    // 初期化
    application.initialize();

    // ゲームループ開始
    return bel::Application::GetInstance().enterLoop();
}
