/*!
 * @file   belMain.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <Windows.h>
// bel
#include "belMain.h"

//-----------------------------------------------------------------------------
/*!
 * エントリーポイント
 * Console
 */
int main(int argc, const char** argv)
{
    return belMain(argc, argv);
}

/*!
 * エントリーポイント
 * Windows
 */
int APIENTRY WinMain(
    _In_     HINSTANCE instance,
    _In_opt_ HINSTANCE prev_instance,
    _In_     LPSTR     p_cmd_line,
    _In_     int       cmd_show
)
{
    return belMain(__argc, const_cast<const char**>(__argv));
}
