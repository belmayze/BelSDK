﻿/*!
 * @file   belIApplicationCallback.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// アプリケーションコールバックのインターフェースクラス
//-----------------------------------------------------------------------------
class IApplicationCallback
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 描画コマンド生成前の計算タイミングで呼ばれます
     */
    virtual void onCalc() = 0;

    /*!
     * 描画コマンドを生成するタイミングで呼ばれます
     * @param[in] command_context コマンドコンテキスト
     */
    virtual void onMakeCommand(gfx::CommandContext& command_context) const = 0;
};
//-----------------------------------------------------------------------------
}