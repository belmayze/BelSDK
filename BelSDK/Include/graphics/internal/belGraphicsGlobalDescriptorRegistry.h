/*!
 * @file   belGraphicsGlobalDescriptorRegistry.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/internal/belGraphicsGlobalDescriptorHandle.h"
#include "memory/belSingleton.h"

namespace bel::gfx { class ConstantBuffer; }
namespace bel::gfx { class Texture; }

namespace bel::gfx {
//-----------------------------------------------------------------------------
// グローバルなデスクリプター登録
//-----------------------------------------------------------------------------
class GlobalDescriptorRegistry : public Singleton<GlobalDescriptorRegistry>
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
    GlobalDescriptorHandle registerTexture(const Texture& texture);

    /*!
     * 定数バッファーを登録する
     * @param[in] location GPU仮想アドレス
     * @param[in] size     バッファーサイズ
     */
    GlobalDescriptorHandle registerConstantBuffer(D3D12_GPU_VIRTUAL_ADDRESS location, size_t size);

    /*!
     * デスクリプターハンドルを取得する
     * @param[in] handle ハンドル
     */
    D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const GlobalDescriptorHandle& handle);

    //-------------------------------------------------------------------------
    // eraser
    //-------------------------------------------------------------------------
public:
    //! 削除できるのはハンドルから
    class EraseAccessor
    {
        friend GlobalDescriptorHandle;
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
