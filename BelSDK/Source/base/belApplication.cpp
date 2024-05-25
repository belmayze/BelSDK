/*!
 * @file   belApplication.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// std
#include <clocale>
// bel
#include "base/belApplication.h"
#include "base/belIApplicationCallback.h"
#include "base/belApplicationWindow.h"
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/belGraphicsEngine.h"
#include "thread/belThread.h"

namespace bel {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Application::Application() {}
//-----------------------------------------------------------------------------
Application::~Application() {}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool Application::initialize(const InitializeArg& arg)
{
    // ウィンドウメッセージ処理スレッドを初期化
    mpWindowMessageThread = std::make_unique<Thread>(
        "[bel] WindowMessage",
        [&arg, this](const Thread& thread)
        {
            return onInvokeWindowMessage(thread, arg);
        }
    );
    mpWindowMessageThread->setAffinityMask(1); // Core 0
    mpWindowMessageThread->start();

    // ウィンドウ生成が終わるまで待機
    mInitializedEvent.wait();

    // グラフィックス初期化
    if (!GraphicsEngine::GetInstance().initialize())
    {
        // 終了処理
        GraphicsEngine::GetInstance().finalize();

        // 失敗したらウィンドウの終了を待つ
        mIsQuit.store(true, std::memory_order_release);
        mQuitEvent.signal();
        mpWindowMessageThread->join();
        return false;
    }

    // コールバック登録
    mpCallback = arg.p_callback;

    return true;
}
//-----------------------------------------------------------------------------
int Application::enterLoop()
{
    // 終了するまで待機
    while (!mIsQuit.load(std::memory_order_acquire))
    {
        // 計測リセット
        debug::PerfTime::GetInstance().reset();
        //debug::PerfTime::GetInstance().start("GPU");

        // Command Queue
        //  -> [ExecuteCommand]
        //   -> CPU
        //    -> Calc
        //   -> GPU
        //    -> [WaitCommandQueue]
        //     -> [Present]
        //      -> Display
        // 前フレームの画面反映
        GraphicsEngine::GetInstance().present();
        
        // コマンドの生成
        {
            GraphicsEngine::ApplicationAccessor accessor;
            accessor.getMainCommandList().begin();
            {
                // コマンドコンテキスト生成
                gfx::CommandContext context(accessor.getMainCommandList());

                // 初回コマンド生成
                GraphicsEngine::GetInstance().makeInitialCommand(context);

                // コールバック
                if (mpCallback) { mpCallback->onMakeCommand(context); }
            }
            accessor.getMainCommandList().end();
        }
        
        // コマンド実行
        GraphicsEngine::GetInstance().executeCommand();

        // 計算処理
        if (mpCallback) { mpCallback->onCalc(); }

        // コマンド実行と VSync 待ち
        GraphicsEngine::GetInstance().waitCommandQueue();
        //debug::PerfTime::GetInstance().end();
    }

    // ウィンドウを閉じる前にグラフィックスの後処理
    GraphicsEngine::GetInstance().finalize();

    // 終了をシグナルして終了を待機
    mQuitEvent.signal();
    mpWindowMessageThread->join();

    return 0;
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
int Application::onInvokeWindowMessage(const Thread& thread, const InitializeArg& arg)
{
    // ウィンドウ生成
    // 生成したスレッドでしかメッセージ処理できないのでここでやる
    if (!ApplicationWindow::GetInstance().createWindow(arg.title, arg.window_class_name, arg.width, arg.height))
    {
        // 失敗したら終了
        mInitializedEvent.signal();
        mIsQuit.store(true, std::memory_order_release);
        return -1;
    }

    // コンテントルートを設定
    ApplicationWindow::GetInstance().setContentRoot(arg.content_root ? arg.content_root : "");

    // ウィンドウ生成が終わったらシグナル
    mInitializedEvent.signal();

    // メッセージ処理
    {
        bool is_quit = false;
        do
        {
            is_quit = ApplicationWindow::GetInstance().peekWindowMessage();
            // 占有しないように sleep を入れる
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        } while (!is_quit && !mIsQuit.load(std::memory_order_acquire));

        mIsQuit.store(true, std::memory_order_release);
    }

    // 終了するまで待機
    mQuitEvent.wait();

    // ウィンドウ終了
    ApplicationWindow::GetInstance().destroyWindow();

    return 0;
}
//-----------------------------------------------------------------------------
}