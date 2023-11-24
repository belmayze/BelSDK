/*!
 * @file   belRenderTarget.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#define NOMINMAX
#include <d3d12.h>
#include <wrl/client.h>

namespace bel::gfx { class Texture; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * レンダーターゲット
 */
class RenderTarget
{
    //-------------------------------------------------------------------------
    // system
    //-------------------------------------------------------------------------
public:
    /*!
     * 初期化
     * @param[in] texture テクスチャー
     */
    bool initialize(const Texture& texture);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //
    ID3D12DescriptorHeap& getDescriptorHeap() { BEL_ASSERT(mpDescriptorHeap); return *mpDescriptorHeap.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
};
//-----------------------------------------------------------------------------

}
