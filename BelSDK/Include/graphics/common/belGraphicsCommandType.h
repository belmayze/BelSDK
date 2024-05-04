/*!
 * @file   belGraphicsCommandType.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx {
//-----------------------------------------------------------------------------
// コマンドリストタイプ
//-----------------------------------------------------------------------------
enum class CommandType
{
    cGraphics, //!< グラフィックスコマンド
    cCompute,  //!< コンピュート専用コマンド
    cCopy,     //!< コピー専用コマンド
    cNum
};
//-----------------------------------------------------------------------------
}
