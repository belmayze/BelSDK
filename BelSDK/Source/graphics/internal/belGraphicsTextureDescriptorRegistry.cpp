/*!
 * @file   belGraphicsTextureDescriptorRegistry.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
bool TextureDescriptorRegistry::allocate(uint32_t num)
{
    // グローバルで大きなテクスチャー用デスクリプターヒープを作る
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = num;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&mpDescriptorHeap))))
        {
            return false;
        }
    }

    // フリーリストの生成
    mFreeIndices.allocate(num);
    for (uint32_t index = 0; index < num; ++index)
    {
        mFreeIndices.emplace_back(index);
    }

    return true;
}
//-----------------------------------------------------------------------------
TextureDescriptorHandle TextureDescriptorRegistry::registerTexture(const Texture& texture)
{
    // フリーリストから取得
    uint32_t index = 0;
    {
        std::lock_guard lock(mFreeListMutex);
        if (mFreeIndices.empty()) { return TextureDescriptorHandle(); }
        index = mFreeIndices.back();
        mFreeIndices.pop_back();
    }

    TextureDescriptorHandle handle;
    TextureDescriptorHandle::Accessor(handle).setId(index);
    return handle;
}
//-----------------------------------------------------------------------------
// EraseAccessor
//-----------------------------------------------------------------------------
void TextureDescriptorRegistry::EraseAccessor::Erase(uint32_t id)
{
    TextureDescriptorRegistry& instance = TextureDescriptorRegistry::GetInstance();

    // フリーリストに登録
    {
        std::lock_guard lock(instance.mFreeListMutex);
        instance.mFreeIndices.emplace_back(id);
    }
}
//-----------------------------------------------------------------------------
}