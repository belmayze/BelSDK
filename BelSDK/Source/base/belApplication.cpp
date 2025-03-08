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
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/common/belGraphicsViewport.h"
#include "graphics/belGraphicsEngine.h"
#include "thread/belThread.h"

namespace bel {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Application::Application() {}
//-----------------------------------------------------------------------------
Application::~Application() {}
//-----------------------------------------------------------------------------
bool Application::initialize(const InitializeArg& arg)
{
    // コンテントルートを設定
    ApplicationWindow::GetInstance().setContentRoot(arg.content_root ? arg.content_root : "");

    // CPU チェック
    {
        int cpu_info[8];
        int cpu_info_ex[4];
        __cpuid(&cpu_info[0], 0);
        __cpuid(&cpu_info[4], 1);
        __cpuidex(&cpu_info_ex[0], 7, 0);

        // AVX2
#       if BEL_SIMD_USE_AVX2()
        if (cpu_info[0] < 7 || (cpu_info[6] & 0x38081001) != 0x38081001 || (cpu_info_ex[1] & 0x20) != 0x20)
        {
            BEL_ERROR_LOG("AVX2 の命令に対応していません");
            return false;
        }
#       endif // BEL_SIMD_USE_AVX2()
    }

    return true;
}
//-----------------------------------------------------------------------------
bool Application::loadSystemResource(const std::string& filepath)
{
    return mSystemResourceHolder.loadSync(filepath);
}
//-----------------------------------------------------------------------------
bool Application::createWindow(const CreateWindowArg& arg)
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

    // グラフィックス生成が終わったらシステムリソースを生成する
    if (!mSystemResourceHolder.createGraphicsResource()) { return false; }

    return true;
}
//-----------------------------------------------------------------------------
int Application::enterLoop(IApplicationCallback* p_callback)
{
    // 終了するまで待機
    while (!mIsQuit.load(std::memory_order_acquire))
    {
        // 計測リセット
        debug::PerfTime::GetInstance().swapBuffer();
        debug::PerfTime::GetInstance().startMainCPU();

        // Command Queue
        //  -> [ExecuteCommand]
        //   -> CPU
        //    -> Calc
        //   -> GPU
        //    -> [WaitCommandQueue]
        //     -> [Present]
        //      -> Display

        // コマンド実行
        GraphicsEngine::GetInstance().executeCommand();

        // 前フレームの画面反映
        GraphicsEngine::GetInstance().present();

        // 計算処理
        GraphicsEngine::GetInstance().update();
        if (p_callback) { p_callback->onCalc(); }
        
        // コマンドの生成
        {
            GraphicsEngine::ApplicationAccessor accessor;
            accessor.getMainCommandList().begin();
            {
                // コマンドコンテキスト生成
                gfx::CommandContext command(accessor.getMainCommandList());

                // デフォルトレンダーターゲットとレンダーバッファーを取得
                gfx::RenderTarget& default_render_target = GraphicsEngine::GetInstance().getDefaultRenderTarget();
                gfx::RenderBuffer& default_render_buffer = GraphicsEngine::GetInstance().getDefaultRenderBuffer();

                // GPU 計測
                debug::PerfTime::GetInstance().startMainGPU(command);
                {
                    // PRESENT -> RENDER_TARGET
                    default_render_target.getTexture().barrierTransition(command, gfx::ResourceState::cRenderTarget);

                    // クリアする
                    default_render_buffer.clear(command, Color::cGray(), 1.f, 0, gfx::EClearType::cColor);

                    // レンダーバッファーセット
                    default_render_buffer.bind(command);

                    // ビューポート
                    gfx::Viewport(default_render_buffer).setCommand(command);

                    // コールバック
                    if (p_callback) { p_callback->onMakeCommand(command); }
                }
                debug::PerfTime::GetInstance().endMainGPU(command);

                // RENDER_TARGET -> PRESENT
                default_render_target.getTexture().barrierTransition(command, gfx::ResourceState::cPresent);
            }
            accessor.getMainCommandList().end();
        }

        // 計測終了
        debug::PerfTime::GetInstance().endMainCPU();

        // コマンド実行と VSync 待ち
        GraphicsEngine::GetInstance().waitCommandQueue();

        // ここでコマンド実行が終わるので計測結果を読み込む
        debug::PerfTime::GetInstance().resolveGPUTimestamp();
    }

    // ウィンドウを閉じる前にグラフィックスの後処理
    bel::debug::PerfTime::GetInstance().finalize();
    mSystemResourceHolder = res::SystemResourceHolder();
    GraphicsEngine::GetInstance().finalize();

    // 終了をシグナルして終了を待機
    mQuitEvent.signal();
    mpWindowMessageThread->join();

    return 0;
}
//-----------------------------------------------------------------------------
// callback
//-----------------------------------------------------------------------------
int Application::onInvokeWindowMessage(const Thread& thread, const CreateWindowArg& arg)
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