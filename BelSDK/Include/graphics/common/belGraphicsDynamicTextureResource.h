/*!
 * @file   belGraphicsDynamicTextureResource.h
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
// 動的確保するテクスチャー
//-----------------------------------------------------------------------------
class DynamicTextureResource : public Singleton<DynamicTextureResource>
{
    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    /*!
     * 全体のテクスチャーメモリーを確保する
     * @param[in] num 最大デスクリプター数
     */
    bool initialize(size_t size);

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12Heap> mpHeap;
    size_t                             mHeapSize = 0;
};
//-----------------------------------------------------------------------------
}
