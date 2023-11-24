/*!
 * @file   belTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belTexture.h"
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

    // フリーリスト作成
    mFreeList.reserve(num);
    for (uint32_t i = 0; i < num; ++i)
    {
        mFreeList.emplace_back(i);
    }

    return true;
}
//-----------------------------------------------------------------------------
// register / erase
//-----------------------------------------------------------------------------
TextureDescriptorHandle TextureDescriptorRegistry::registerTexture(const Texture& texture)
{
    uint32_t id = 0;
    {
        // 登録先の ID を検索
        std::lock_guard lock(mListMutex);
        if (mFreeList.empty())
        {
            // 登録の上限に達した場合は無効値を返す
            return TextureDescriptorHandle();
        }

        id = mFreeList.back();
        mFreeList.pop_back();
    }

    // 登録
    D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
    desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    desc.Format                  = texture.getFormat();

    // dimension 変換
    switch (texture.getDimension())
    {
    case Texture::Dimension_1D:
        if (texture.getDepth() == 1)
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
            desc.Texture1D.MipLevels = texture.getNumMip();
        }
        else
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
            desc.Texture1DArray.MipLevels = texture.getNumMip();
            desc.Texture1DArray.ArraySize = texture.getDepth();
        }
        break;

    case Texture::Dimension_2D:
        if (texture.getDepth() == 1)
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipLevels = texture.getNumMip();
        }
        else
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            desc.Texture2DArray.MipLevels = texture.getNumMip();
            desc.Texture2DArray.ArraySize = texture.getDepth();
        }
        break;

    case Texture::Dimension_Cube:
        if (texture.getDepth() == 6)
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
            desc.TextureCube.MipLevels = texture.getNumMip();
        }
        else
        {
            desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
            desc.TextureCubeArray.MipLevels = texture.getNumMip();
            desc.TextureCubeArray.NumCubes = texture.getDepth() / 6;
        }
        break;

    case Texture::Dimension_3D:
        desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
        desc.Texture3D.MipLevels = texture.getNumMip();
        break;
    }

    // SRV 作成
    D3D12_CPU_DESCRIPTOR_HANDLE handle = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    handle.ptr += Graphics::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * id;
    Graphics::GetInstance().getDevice().CreateShaderResourceView(&texture.getResource(), &desc, handle);

    // ハンドル生成
    TextureDescriptorHandle retval;
    TextureDescriptorHandle::Accessor accessor(retval);
    accessor.setId(id);

    return retval;
}
//-----------------------------------------------------------------------------
void TextureDescriptorRegistry::EraseAccessor::Erase(uint32_t id)
{
    TextureDescriptorRegistry& instance = TextureDescriptorRegistry::GetInstance();

    // SRV 削除は不要？

    // フリーリストに戻す
    {
        std::lock_guard lock(instance.mListMutex);
        instance.mFreeList.emplace_back(id);
    }
}
//-----------------------------------------------------------------------------
TextureDescriptorRegistry::TextureDescriptorRegistry() {}
TextureDescriptorRegistry::~TextureDescriptorRegistry() {}
//-----------------------------------------------------------------------------
}
