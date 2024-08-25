/*!
 * @file   belGraphicsDynamicDescriptorHeap.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
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
     * テクスチャー用デスクリプターヒープの確保を行う
     * @param[in] num 最大デスクリプター数
     */
    bool initialize(uint32_t num);

    //-------------------------------------------------------------------------
    // allocate
    //-------------------------------------------------------------------------
public:

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    std::atomic_uint32_t mDescriptorIndex = 0;
    uint32_t             mDescriptorSize  = 0;
    uint32_t             mBufferIndex     = 0;
};
//-----------------------------------------------------------------------------
}
