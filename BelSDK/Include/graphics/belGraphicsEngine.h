/*!
 * @file   belGraphicsEngine.h
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
class IGraphicsEngine
{
    //-------------------------------------------------------------------------
public:
    /*!
     * グラフィックスを初期化します
     */
    virtual bool initialize() = 0;

    /*!
     * メインキューのコマンドを実行します
     */
    virtual void executeCommand() = 0;

    /*!
     * メインキューのコマンド実行完了を待機する
     */
    virtual void waitCommandQueue() = 0;

    /*!
     * 画面を反映する
     */
    virtual void present() = 0;

    /*!
     * グラフィックスを終了処理します
     */
    virtual void finalize() = 0;
};
//-----------------------------------------------------------------------------
}

// 各プラットフォーム
#include "graphics/belGraphicsEngineD3D.h"

namespace bel {

class GraphicsEngineD3D;

#if BEL_PLATFORM_IS_WIN()
using GraphicsEngine = GraphicsEngineD3D;
#endif // BEL_PLATFORM_IS_...

}
