/*!
 * @file   belDefine.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

// DEBUG / RELEASE
#ifdef _DEBUG
#   define BEL_DEBUG    (1)
#   define BEL_RELEASE  (0)
#else
#   define BEL_DEBUG    (0)
#   define BEL_RELEASE  (1)
#endif // _DEBUG

#define BEL_IS_DEBUG()   (BEL_DEBUG   == 1)
#define BEL_IS_RELEASE() (BEL_RELEASE == 1)

// include
#include "io/belErrorWindow.h"

// ErrorWindow
#if BEL_IS_DEBUG()
#   define BEL_ERROR_WINDOW(title, message) { bel::io::ErrorWindow::GetInstance().show(title, message); }
#else
#   define BEL_ERROR_WINDOW(title, message) {}
#endif // BEL_IS_DEBUG()

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xinput.lib")
