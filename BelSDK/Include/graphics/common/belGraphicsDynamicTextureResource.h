/*!
 * @file   belGraphicsDynamicTextureResource.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "memory/belSingleton.h"
#include "memory/belMemoryBlockAllocator.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// 動的確保するテクスチャー
//-----------------------------------------------------------------------------
class DynamicTextureResource : public Singleton<DynamicTextureResource>
{
    //-------------------------------------------------------------------------
public:
    //! アロケート引数
    struct AllocateArg
    {
        uint32_t         width      = 1;
        uint32_t         height     = 1;
        uint32_t         depth      = 1;
        uint32_t         mip_levels = 1;
        uint32_t         component_mapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        TextureFormat    format            = TextureFormat::cR8G8B8A8_uNorm;
        TextureDimension dimension         = TextureDimension::c2D;

        Color   optimized_clear_color   = Color::cBlack();
        float   optimized_clear_depth   = 1.f;
        uint8_t optimized_clear_stencil = 0;
    };

    //-------------------------------------------------------------------------
    // memory
    //-------------------------------------------------------------------------
public:
    /*!
     * 全体のテクスチャーメモリーを確保する
     * @param[in] size 確保するメモリーサイズ
     */
    bool initialize(size_t size);

    /*!
     * テクスチャーリソース確保
     * @param[in] arg 引数
     */
    Texture allocate(const AllocateArg& arg);

    /*!
     * フリー
     * @param[in] texture 解放するテクスチャー
     */
    void free(Texture&& texture);

    //-------------------------------------------------------------------------
private:
    //! キーバリュー
    using MemoryKeyValue = std::pair<const void*, size_t>;

    //-------------------------------------------------------------------------
private:
    MemoryBlockAllocator               mAllocator;
    List<MemoryKeyValue>               mAllocatedList;
    Microsoft::WRL::ComPtr<ID3D12Heap> mpHeap;
    size_t                             mHeapSize = 0;
};
//-----------------------------------------------------------------------------
}
