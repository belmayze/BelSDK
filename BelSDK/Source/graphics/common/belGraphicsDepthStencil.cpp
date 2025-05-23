﻿/*!
 * @file   belGraphicsDepthStencil.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsDepthStencil.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool DepthStencil::initialize()
{
    // デスクリプターヒープ作成
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = 1;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            BEL_ERROR_LOG("デプスステンシルのデスクリプターヒープの生成に失敗しました\n");
            return false;
        }
    }
    mpDescriptorHeap = std::move(p_descriptor_heap);

    return true;
}
//-----------------------------------------------------------------------------
void DepthStencil::updateTexture(const Texture& texture)
{
    // リソースフラグチェック
    BEL_ASSERT_MSG(texture.getResourceFlags().test(ResourceFlag::cDepthStencil), "DepthStencilに指定できません");

    // テクスチャー保持
    mpTexture = &texture;

    // デスクリプターヒープからデプスステンシルを作成
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
        desc.Format = to_native(mpTexture->getFormat());
        desc.ViewDimension = to_native_dsv(mpTexture->getDimension());
        GraphicsEngine::GetInstance().getDevice().CreateDepthStencilView(
            &mpTexture->getResource(),
            &desc,
            mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
        );
    }
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void DepthStencil::clear(CommandContext& command, float depth, uint8_t stencil, EnumFlags<EClearType> clear_flags) const
{
    // フラグチェック
    D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAGS();
    if (clear_flags.test(EClearType::cDepth))   { flags |= D3D12_CLEAR_FLAG_DEPTH; }
    if (clear_flags.test(EClearType::cStencil)) { flags |= D3D12_CLEAR_FLAG_STENCIL; }

    command.getCommandList().ClearDepthStencilView(
        mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
        flags, depth, stencil,
        0, nullptr
    );
}
//-----------------------------------------------------------------------------
}