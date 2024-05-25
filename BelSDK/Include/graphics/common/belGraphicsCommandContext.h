/*!
 * @file   belGraphicsCommandContext.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::gfx { class CommandList; }
namespace bel::gfx { class CommandQueue; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// コマンドコンテキスト
//-----------------------------------------------------------------------------
class CommandContext
{
    //-------------------------------------------------------------------------
public:
    //! コンストラクター
    CommandContext(CommandList& command_list) : mCommandList(command_list) {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! コマンドリストを取得する
    ID3D12GraphicsCommandList& getCommandList();

    //-------------------------------------------------------------------------
private:
    CommandList& mCommandList;
};
//-----------------------------------------------------------------------------
}
