/*!
 * @file   belGraphicsDefine.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx
{

//! クリア種別
enum class EClearType
{
    cColor,
    cDepth,
    cStencil,
    cNum
};

//! レンダーターゲットの最大数
constexpr size_t cMaxRenderTargets = 8;

}
