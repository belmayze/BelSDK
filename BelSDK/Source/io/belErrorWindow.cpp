/*!
 * @file   belErrorWindow.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <tchar.h>
#define NOMINMAX
#include <Windows.h>
// bel
#include "io/belErrorWindow.h"

namespace bel::io
{
//-----------------------------------------------------------------------------
void ErrorWindow::show(const std::string& title, const std::string& message)
{
    size_t title_strlen   = title.length() + 1;
    size_t message_strlen = message.length() + 1;
    std::unique_ptr<TCHAR[]> title_t = std::make_unique<TCHAR[]>(title_strlen);
    std::unique_ptr<TCHAR[]> message_t = std::make_unique<TCHAR[]>(message_strlen);

    size_t ret = 0;
#   ifdef UNICODE
    mbstowcs_s(&ret, title_t.get(), title_strlen, title.c_str(), title_strlen);
    mbstowcs_s(&ret, message_t.get(), message_strlen, message.c_str(), message_strlen);
#   else
    strcpy_s(title_t.get(), title_strlen, title.c_str());
    strcpy_s(message_t.get(), message_strlen, message.c_str());
#   endif // UNICODE

    MessageBox(nullptr, message_t.get(), title_t.get(), 0);
}
//-----------------------------------------------------------------------------
}
