/*!
 * @file   belGraphicsTextureDescriptorRegistry.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/internal/belGraphicsTextureDescriptorHandle.h"
#include "memory/belSingleton.h"

namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// テクスチャー登録
//-----------------------------------------------------------------------------
class TextureDescriptorRegistry : public Singleton<TextureDescriptorRegistry>
{
    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    /*!
     * テクスチャー用デスクリプターヒープの確保を行う
     * @param[in] num 最大デスクリプター数
     */
    bool allocate(uint32_t num);

    /*!
     * テクスチャーを登録する
     * @param[in] texture 登録するテクスチャー
     */
    TextureDescriptorHandle registerTexture(const Texture& texture);

    //-------------------------------------------------------------------------
    // eraser
    //-------------------------------------------------------------------------
public:
    //! 削除できるのはハンドルから
    class EraseAccessor
    {
        friend TextureDescriptorHandle;
    private:
        static void Erase(uint32_t id);
    };

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    Vector<uint32_t> mFreeIndices;
    std::mutex       mFreeListMutex;
};
//-----------------------------------------------------------------------------
}
