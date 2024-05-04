/*!
 * @file   belGraphicsRenderTarget.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool RenderTarget::initialize(const Texture& texture)
{
    // テクスチャー保持
    mpTexture = &texture;

    // デスクリプターヒープ作成
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 1;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            BEL_PRINT("レンダーターゲットのデスクリプターヒープの生成に失敗しました\n");
            return false;
        }
    }
    mpDescriptorHeap = std::move(p_descriptor_heap);

    // デスクリプターヒープからレンダーターゲットを作成
    {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format        = to_native(texture.getFormat());
        desc.ViewDimension = to_native_rtv(texture.getDimension());
        GraphicsEngine::GetInstance().getDevice().CreateRenderTargetView(
            &texture.getResource(),
            &desc,
            mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
        );
    }

    return true;
}
//-----------------------------------------------------------------------------
}