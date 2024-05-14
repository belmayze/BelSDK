/*!
 * @file   belConsole.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "debug/belConsole.h"

namespace bel::debug {
//-----------------------------------------------------------------------------
void Console::Output(const char* format, ...)
{
    //
    constexpr size_t cMaxSize = 1024;

    // string
    std::unique_ptr<char[]> string_buffer = std::make_unique<char[]>(cMaxSize);

    va_list va;
    va_start(va, format);
#   if BEL_PLATFORM_IS_WIN
    vsnprintf_s(string_buffer.get(), cMaxSize, cMaxSize, format, va);
#   else
    vsnprintf(string_buffer.get(), cMaxSize, format, va);
#   endif
    va_end(va);

    // string -> tcahr
#   if BEL_PLATFORM_IS_WIN()
    std::unique_ptr<TCHAR[]> tchar = std::make_unique<TCHAR[]>(cMaxSize);

    size_t ret = 0;
#   ifdef UNICODE
    mbstowcs_s(&ret, tchar.get(), cMaxSize, string_buffer.get(), _TRUNCATE);
#   else
    strcpy_s(tchar.get(), cMaxSize, string_buffer.get());
#   endif

    OutputDebugString(tchar.get());
#   endif // BEL_PLATFORM_IS_WIN
}
//-----------------------------------------------------------------------------
}
