﻿/*!
 * @file   belDefine.h
 * @brief
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

//-----------------------------------------------------------------------------
// platform
#define BEL_PLATFORM_WIN (0)

#ifdef _WIN32
#   define BEL_PLATFORM (BEL_PLATFORM_WIN)
#else
#   error "Unsupported platform."
#endif //

#define BEL_PLATFORM_IS_WIN() (BEL_PLATFORM == BEL_PLATFORM_WIN)

//-----------------------------------------------------------------------------
// SIMD
#define BEL_SIMD_USE_AVX()  (1)
#define BEL_SIMD_USE_AVX2() (1 && BEL_SIMD_USE_AVX())

//-----------------------------------------------------------------------------
// library
#define BEL_GRAPHICS_D3D (0)

#if BEL_PLATFORM_IS_WIN()
#   define BEL_GRAPHICS (BEL_GRAPHICS_D3D)
#else
#   error "Unsupported platform."
#endif // BEL_PLATFORM_IS_***()

#define BEL_GRAPHICS_IS_D3D() (BEL_GRAPHICS == BEL_GRAPHICS_D3D)

//-----------------------------------------------------------------------------
// target
#define BEL_TARGET_DEBUG   (0)
#define BEL_TARGET_RELEASE (1)

#ifdef _DEBUG
#   define BEL_TARGET (BEL_TARGET_DEBUG)
#else
#   define BEL_TARGET (BEL_TARGET_RELEASE)
#endif // _DEBUG

#define BEL_TARGET_IS_DEBUG()   (BEL_TARGET == BEL_TARGET_DEBUG)
#define BEL_TARGET_IS_RELEASE() (BEL_TARGET == BEL_TARGET_RELEASE)
