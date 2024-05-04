/*!
 * @file   belApplication.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "memory/belSingleton.h"
#include "thread/belEvent.h"

namespace bel { class Thread; }

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションの処理を行います
//-----------------------------------------------------------------------------
class Application : public Singleton<Application>
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        uint32_t    width        = 640;
        uint32_t    height       = 480;
        const char* title        = nullptr;
        const char* window_class = nullptr;
    };

    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    Application();
    //! デストラクター
    ~Application();

    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化処理
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    /*!
     * ループ処理
     * @note この関数を呼び出すとアプリが終了するまで返ってきません
     * @return 終了コード
     */
    int enterLoop();

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<Thread> mpWindowMessageThread;
    std::atomic_bool        mIsQuit = false;
    Event                   mInitializedEvent;
    Event                   mQuitEvent;

    //-------------------------------------------------------------------------
    // callback
    //-------------------------------------------------------------------------
private:
    /*!
     * ウィンドウメッセージ処理スレッド
     * @param[in] thread 実行スレッド
     * @param[in] arg    初期化引数
     */
    int onInvokeWindowMessage(const Thread& thread, const InitializeArg& arg);
};
//-----------------------------------------------------------------------------
}
