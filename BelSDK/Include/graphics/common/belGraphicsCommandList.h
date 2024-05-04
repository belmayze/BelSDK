/*!
 * @file   belGraphicsCommandList.h
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
// コマンドリスト
//-----------------------------------------------------------------------------
class CommandList
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
    // recording
    //-------------------------------------------------------------------------
public:
    /*!
     * 記録を開始します
     */
    void begin();

    /*!
     * 記録を終了します
     */
    void end();

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! コマンドリスト
    ID3D12GraphicsCommandList& getCommandList() { BEL_ASSERT(mpCommandList.Get() != nullptr); return *mpCommandList.Get(); }

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator>    mpAllocator;
    Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mpCommandList;
};
//-----------------------------------------------------------------------------
}
