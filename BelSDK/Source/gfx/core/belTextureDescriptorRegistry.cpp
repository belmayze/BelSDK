/*!
 * @file   belTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belTextureDescriptorRegistry.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool TextureDescriptorRegistry::allocate(uint32_t num)
{
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;

    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = num;
    desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (FAILED(Graphics::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
    {
        return false;
    }
    mpDescriptorHeap = std::move(p_descriptor_heap);

    return true;
}
//-----------------------------------------------------------------------------
TextureDescriptorRegistry::TextureDescriptorRegistry() {}
TextureDescriptorRegistry::~TextureDescriptorRegistry() {}
//-----------------------------------------------------------------------------
}
