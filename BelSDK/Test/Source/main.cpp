/*!
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

// DirectX 12 Agility SDK
extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 615; }
extern "C" { __declspec(dllexport) extern const char8_t* D3D12SDKPath = u8".\\D3D12\\"; }

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
        init_arg.content_root = p_content;

        if (!bel::Application::GetInstance().initialize(init_arg)) { return -1; }
    }

    // ファイル読み込み
    if (!bel::Application::GetInstance().loadSystemResource("")) { return -1; }

    // ウィンドウ生成
    {
        bel::Application::CreateWindowArg create_window_arg;
        create_window_arg.width             = 1280;
        create_window_arg.height            = 720;
        create_window_arg.title             = "BelTest";
        create_window_arg.window_class_name = "belmayze";

        if (!bel::Application::GetInstance().createWindow(create_window_arg)) { return -1; }
    }

    // 初期化
    application.initialize();

    // ゲームループ開始
    return bel::Application::GetInstance().enterLoop(&application);
}
