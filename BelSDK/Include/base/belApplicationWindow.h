/*!
 * @file   belApplicationWindow.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションのウィンドウ処理を行います
//-----------------------------------------------------------------------------
class IApplicationWindow
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
    virtual bool createWindow(const char* title, const char* class_name, uint32_t width, uint32_t height) = 0;

    /*!
     * メッセージを処理します
     * @note true が返ってきた場合はアプリケーションループを終了させます
     * @return 終了メッセージが来たら true を返します
     */
    virtual bool peekWindowMessage() = 0;

    /*!
     * ウィンドウを破棄します
     */
    virtual void destroyWindow() = 0;

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! ウィンドウレンダリング領域の横幅を取得します
    virtual uint32_t getRenderWidth() const = 0;
    //! ウィンドウレンダリング領域の高さを取得します
    virtual uint32_t getRenderHeight() const = 0;
};
//-----------------------------------------------------------------------------
}

// 各プラットフォーム
#include "base/belApplicationWindowWin.h"

namespace bel {

class ApplicationWindowWin;

#if BEL_PLATFORM_IS_WIN()
using ApplicationWindow = ApplicationWindowWin;
#endif // BEL_PLATFORM_IS_...

}
