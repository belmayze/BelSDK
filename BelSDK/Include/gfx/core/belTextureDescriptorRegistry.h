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
#include <mutex>
#include <optional>
#include <vector>
#include <wrl/client.h>
// bel
#include "gfx/core/belTextureDescriptorHandle.h"
#include "memory/belSingleton.h"

namespace bel::gfx { class Texture; }

namespace bel::gfx
{

//-----------------------------------------------------------------------------
/*!
 * テクスチャーデスクリプター
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

    //-------------------------------------------------------------------------
    // register
    //-------------------------------------------------------------------------
public:
    /*!
     * 登録
     * @param[in] texture 登録するテクスチャー
     */
    TextureDescriptorHandle registerTexture(const Texture& texture);

    //-------------------------------------------------------------------------
    // erase
    //-------------------------------------------------------------------------
public:
    class EraseAccessor
    {
        friend TextureDescriptorHandle;
    private:
        static void Erase(uint32_t id);
    };

    //-------------------------------------------------------------------------
private:
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mpDescriptorHeap;
    std::vector<uint32_t> mFreeList;
    std::mutex            mListMutex;

    //-------------------------------------------------------------------------
private:
    friend class Singleton<TextureDescriptorRegistry>;
    TextureDescriptorRegistry();
    ~TextureDescriptorRegistry();
};
//-----------------------------------------------------------------------------

}
