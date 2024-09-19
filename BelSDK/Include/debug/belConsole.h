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
#if 1//BEL_TARGET_IS_DEBUG()
#   define BEL_PRINT(...)   bel::debug::Console::Output(__VA_ARGS__)
#   define BEL_ERROR(...)   bel::debug::Console::Error(__VA_ARGS__)
#else
#   define BEL_PRINT(...)
#   define BEL_ERROR(...)
#endif // BEL_TARGET_IS_DEBUG()
//-----------------------------------------------------------------------------
