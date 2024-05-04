/*!
 * @file   belGraphicsCommandQueue.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsCommandType.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// コマンドキュー
//-----------------------------------------------------------------------------
class CommandQueue
{
    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化します
     * @param[in] type コマンドの種別
     */
    bool initialize(CommandType type);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! コマンドキュー
    ID3D12CommandQueue& getCommandQueue() { BEL_ASSERT(mpCommandQueue.Get() != nullptr); return *mpCommandQueue.Get(); }

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> mpCommandQueue;
};
//-----------------------------------------------------------------------------
}
