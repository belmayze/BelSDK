/*!
 * @file   belErrorWindow.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <string>
// bel
#include "memory/belSingleton.h"

namespace bel::io
{
//-----------------------------------------------------------------------------
/*!
 * エラーウィンドウを表示します
 */
class ErrorWindow : public Singleton<ErrorWindow>
{
public:
    /*!
     * 表示
     * @param[in] title   タイトル
     * @param[in] message メッセージ
     */
    void show(const std::string& title, const std::string& message);
};
//-----------------------------------------------------------------------------
}
