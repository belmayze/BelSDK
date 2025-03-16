/*!
 * @file   belGraphicsDynamicDescriptorHeap.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsDynamicDescriptorHandle.h"
#include "memory/belSingleton.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// 動的確保するデスクリプターヒープ
//-----------------------------------------------------------------------------
class DynamicDescriptorHeap : public Singleton<DynamicDescriptorHeap>
{
    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    /*!
     * 全体で使用するデスクリプターヒープの確保を行う
     * @param[in] num 最大デスクリプター数
     */
    bool initialize(uint32_t num);

    /*!
     * バッファーの切り替え
     */
    void swapBuffer();

    //-------------------------------------------------------------------------
    // allocate
    //-------------------------------------------------------------------------
public:
    /*!
     * 指定した数を確保した動的デスクリプターハンドルを取得する
     * @param[in] num 必要な個数
     */
    DynamicDescriptorHandle allocate(uint32_t num);

    //-------------------------------------------------------------------------
    // command
    //-------------------------------------------------------------------------
public:
    /*!
     * デスクリプターヒープを設定する
     * @param[in] handle  ハンドル
     * @param[in] command コマンド
     */
    void setDescriptorHeap(DynamicDescriptorHandle& handle, CommandContext& command) const;

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    std::atomic_uint32_t mDescriptorIndex = 0;
    uint32_t             mDescriptorSize  = 0;
    uint32_t             mBufferIndex     = 0;
};
//-----------------------------------------------------------------------------
}
