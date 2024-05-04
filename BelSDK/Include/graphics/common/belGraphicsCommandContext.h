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
    // execute
    //-------------------------------------------------------------------------
public:
    /*!
     * 作成したコマンドを実行します
     * @param[in] command_queue 実行先のキュー
     */
    void executeCommand(CommandQueue& command_queue);

    //-------------------------------------------------------------------------
private:
    CommandList& mCommandList;
};
//-----------------------------------------------------------------------------
}
