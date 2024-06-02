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
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
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
                gfx::CommandContext command(accessor.getMainCommandList());

                // 初回コマンド生成
                GraphicsEngine::GetInstance().makeInitialCommand(command);

                // デフォルトレンダーターゲットを取得
                gfx::RenderTarget& default_render_target = GraphicsEngine::GetInstance().getDefaultRenderTarget();

                // PRESENT -> RENDER_TARGET
                {
                    D3D12_RESOURCE_BARRIER desc = {};
                    desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                    desc.Transition.pResource   = &default_render_target.getTexture().getResource();
                    desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
                    desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
                    command.getCommandList().ResourceBarrier(1, &desc);
                }

                // クリアする
                default_render_target.clear(command, Color::cGray());

                // レンダーバッファー設定
                {
                    D3D12_CPU_DESCRIPTOR_HANDLE handle[1];
                    handle[0] = default_render_target.getDescriptorHandle();
                    command.getCommandList().OMSetRenderTargets(1, handle, FALSE, nullptr);
                }
                // ビューポート
                {
                    const gfx::Texture& texture = default_render_target.getTexture();

                    D3D12_VIEWPORT vp;
                    vp.TopLeftX = 0;
                    vp.TopLeftY = 0;
                    vp.Width    = static_cast<float>(texture.getWidth());
                    vp.Height   = static_cast<float>(texture.getHeight());
                    vp.MinDepth = 0;
                    vp.MaxDepth = 1;
                    command.getCommandList().RSSetViewports(1, &vp);

                    D3D12_RECT rect = {};
                    rect.left   = 0;
                    rect.top    = 0;
                    rect.right  = texture.getWidth();
                    rect.bottom = texture.getHeight();
                    command.getCommandList().RSSetScissorRects(1, &rect);
                }

                // コールバック
                if (mpCallback) { mpCallback->onMakeCommand(command); }

                // RENDER_TARGET -> PRESENT
                {
                    D3D12_RESOURCE_BARRIER desc = {};
                    desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                    desc.Transition.pResource   = &default_render_target.getTexture().getResource();
                    desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
                    desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
                    command.getCommandList().ResourceBarrier(1, &desc);
                }
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