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
};
//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// define
//-----------------------------------------------------------------------------
#if BEL_TARGET_IS_DEBUG()
#   define BEL_PRINT(...)   bel::debug::Console::Output(__VA_ARGS__)
#else
#   define BEL_PRINT(...)
#endif // BEL_TARGET_IS_DEBUG()
//-----------------------------------------------------------------------------
