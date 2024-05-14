/*!
 * @file   belPrecompile.h
 * @brief  必ず include されるヘッダー
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

// std
#include <array>        // array
#include <atomic>       // atomic
#include <cstdint>      // 型
#include <cassert>      // assert
#include <functional>   // ラムダ式
#include <memory>       // スマートポインター
#include <mutex>        // mutex
#include <shared_mutex> // read-write lock
#include <optional>     // optional
#include <string>       // 文字列
#include <thread>       // スレッド

// bel base
#include "belDefine.h"

// platforms
#if BEL_PLATFORM_IS_WIN()
#   include <tchar.h>      // TCHAR
#   define NOMINMAX
#   include <Windows.h>    // Windows
#   include <wrl/client.h> // ComPtr
#endif // BEL_PLATFORM_IS_WIN()

// graphics
#if BEL_GRAPHICS_IS_D3D()
#   include <d3d12.h>
#   include <dxgi1_6.h>
#endif // BEL_GRAPHICS_IS_D3D

// bel
#include "util/belUtilEnum.h"
#include "debug/performance/belPerfTime.h"
#include "debug/belConsole.h"
// bel/math
#include "math/belMath.h"
// bel/container
#include "container/belDeque.h"
#include "container/belVector.h"
// bel/graphics
#include "graphics/common/belGraphicsCommandContext.h"

// ライブラリーリンク
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xinput.lib")
