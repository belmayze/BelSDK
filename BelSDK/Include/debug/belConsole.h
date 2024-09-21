/*!
 * @file   belConsole.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::debug {
//-----------------------------------------------------------------------------
class Console
{
    //-------------------------------------------------------------------------
public:
    /*!
     * 文字列出力
     * @param[in] format  フォーマット
     * @param[in] args... 引数
     */
    static void Output(const char* format, ...);

    /*!
     * エラー出力
     * @param[in] format  フォーマット
     * @param[in] args... 引数
     */
    static void Error(const char* format, ...);
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// define
//-----------------------------------------------------------------------------
//! 標準出力
#define BEL_INFO_LOG(...) bel::debug::Console::Output(__VA_ARGS__)
//! エラー出力
#define BEL_ERROR_LOG(...) bel::debug::Console::Error(__VA_ARGS__)

//! デバッグ出力
#if BEL_TARGET_IS_DEBUG()
#   define BEL_DEBUG_LOG(...) bel::debug::Console::Output(__VA_ARGS__)
#else
#   define BEL_DEBUG_LOG(...)
#endif // BEL_TARGET_IS_DEBUG()
//-----------------------------------------------------------------------------
