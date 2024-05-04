/*!
 * @file   belApplicationWindowWin.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "base/belApplicationWindow.h"
#include "memory/belSingleton.h"

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションのウィンドウ処理を行います
//-----------------------------------------------------------------------------
class ApplicationWindowWin : public IApplicationWindow,
                             public Singleton<ApplicationWindowWin>
{
    //-------------------------------------------------------------------------
public:
    /*!
     * ウィンドウを生成します
     * @param[in] title      タイトル
     * @param[in] class_name ウィンドウクラス名
     * @param[in] width      横幅
     * @param[in] height     高さ
     */
    virtual bool createWindow(const char* title, const char* class_name, uint32_t width, uint32_t height) override;

    /*!
     * メッセージを処理します
     * @note true が返ってきた場合はアプリケーションループを終了させます
     * @return 終了メッセージが来たら true を返します
     */
    virtual bool peekWindowMessage() override;

    /*!
     * ウィンドウを破棄します
     */
    virtual void destroyWindow() override;

    //-------------------------------------------------------------------------
    // getter (override)
    //-------------------------------------------------------------------------
public:
    //! ウィンドウレンダリング領域の横幅を取得します
    virtual uint32_t getRenderWidth() const { return mRenderWidth; }
    //! ウィンドウレンダリング領域の高さを取得します
    virtual uint32_t getRenderHeight() const { return mRenderHeight; }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! HWND
    HWND getWindowHandle() const { return mpWindowHandle; }

    //-------------------------------------------------------------------------
private:
    HWND     mpWindowHandle = HWND();
    uint32_t mRenderWidth   = 0;
    uint32_t mRenderHeight  = 0;
};
//-----------------------------------------------------------------------------
}
