/*!
 * @file   belTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <d3d12.h>
#include <wrl/client.h>
// bel
#include "memory/belSingleton.h"

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * テクスチャー
 */
class TextureDescriptorRegistry : public Singleton<TextureDescriptorRegistry>
{
    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] num 個数
     */
    bool allocate(uint32_t num);

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;

private:
    friend class Singleton<TextureDescriptorRegistry>;
    TextureDescriptorRegistry();
    ~TextureDescriptorRegistry();
};
//-----------------------------------------------------------------------------

}
