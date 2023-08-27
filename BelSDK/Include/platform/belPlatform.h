/*!
 * @file   belPlatform.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <memory>
#include <string>
#include <thread>
#include <Windows.h>
// bel
#include "memory/belSingleton.h"

namespace bel
{

//-----------------------------------------------------------------------------
/*!
 * プラットフォームの内部処理を行います
 */
class Platform : public Singleton<Platform>
{
public:
    /*!
     * ウィンドウを生成する
     * @param[in] title ウィンドウ名
     */
    void createWindow(const std::string& title);

    /*!
     * アプリケーションループに入ります
     * @note この関数を呼ぶと、setExit() を呼ぶまで返ってきません
     */
    void enterApplicationLoop();

    /*!
     * 終了すべきなら呼び出します
     */
    void setExit(int exit_code) { mExit = true; mExitCode = exit_code; }

    /*!
     * 終了すべきかチェック
     */
    bool isExit() const { return mExit; }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! ウィンドウハンドル
    const HWND& getWindowHandle() const { return mWindowHandle; }
    //! 横幅
    uint32_t getWindowWidth() const { return mWidth; }
    //! 高さ
    uint32_t getWindowHeight() const { return mHeight; }

    //-------------------------------------------------------------------------
private:
    HWND mWindowHandle = HWND();
    std::unique_ptr<std::thread> mpWindowMsgThread;

    uint32_t mWidth  = 1920;
    uint32_t mHeight = 1080;

    bool mExit     = true;
    int  mExitCode = 0;

    //-------------------------------------------------------------------------
private:
    /*!
     * アプリケーションループ
     */
    void applicationLoop_();

private:
    friend class Singleton<Platform>;
    Platform() {};
};
//-----------------------------------------------------------------------------

}
