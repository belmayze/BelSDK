/*!
 * @file   belRenderTarget.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belRenderTarget.h"
#include "gfx/core/belTexture.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool RenderTarget::initialize(const Texture& texture)
{
    // デスクリプターヒープを作成
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 1;
        if (FAILED(Graphics::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "レンダーターゲットのデスクリプターヒープの生成に失敗しました");
            return false;
        }
    }

    // デスクリプターヒープからレンダーターゲットを作成
    {
        D3D12_RENDER_TARGET_VIEW_DESC desc = {};
        desc.Format        = texture.getFormat();
        desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
        Graphics::GetInstance().getDevice().CreateRenderTargetView(
            &texture.getResource(),
            &desc,
            p_descriptor_heap->GetCPUDescriptorHandleForHeapStart()
        );
    }
    mpDescriptorHeap = std::move(p_descriptor_heap);

    return true;
}
//-----------------------------------------------------------------------------
}
