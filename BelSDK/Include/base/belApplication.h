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
#include "resource/belSystemResourceHolder.h"
#include "thread/belEvent.h"

namespace bel { class IApplicationCallback; }
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
        const char* content_root = nullptr;
    };

    //! 生成引数
    struct CreateWindowArg
    {
        uint32_t    width             = 640;
        uint32_t    height            = 480;
        const char* title             = nullptr;
        const char* window_class_name = nullptr;
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
     * 初期化
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    /*!
     * システムリソースを読み込みます
     * @param[in] filepath ファイルパス
     */
    bool loadSystemResource(const std::string& filepath);

    /*!
     * ウィンドウを生成します
     * @param[in] arg 生成引数
     */
    bool createWindow(const CreateWindowArg& arg);

    /*!
     * ループ処理
     * @note この関数を呼び出すとアプリが終了するまで返ってきません
     * @param[in] p_callback コールバック関数
     * @return 終了コード
     */
    int enterLoop(IApplicationCallback* p_callback);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    /*!
     * システムリソースを取得する
     */
    const res::SystemResourceHolder& getSystemResource() const { return mSystemResourceHolder; }

    //-------------------------------------------------------------------------
private:
    std::unique_ptr<Thread>   mpWindowMessageThread;
    std::atomic_bool          mIsQuit    = false;
    Event                     mInitializedEvent;
    Event                     mQuitEvent;
    res::SystemResourceHolder mSystemResourceHolder;

    //-------------------------------------------------------------------------
    // callback
    //-------------------------------------------------------------------------
private:
    /*!
     * ウィンドウメッセージ処理スレッド
     * @param[in] thread 実行スレッド
     * @param[in] arg    初期化引数
     */
    int onInvokeWindowMessage(const Thread& thread, const CreateWindowArg& arg);
};
//-----------------------------------------------------------------------------
}
