/*!
 * @file   belImage.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"

namespace bel {
//-----------------------------------------------------------------------------
// 画像
//-----------------------------------------------------------------------------
class Image
{
    //-------------------------------------------------------------------------
public:
    //! 初期化引数
    struct InitializeArg
    {
        size_t   width             = 0;
        size_t   height            = 0;
        size_t   depth             = 0;
        size_t   mip_levels        = 0;
        uint32_t component_mapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        gfx::TextureFormat    format     = gfx::TextureFormat::cUnknown;
        gfx::TextureDimension dimenstion = gfx::TextureDimension::c2D;
    };
    //! 画像データ
    struct ImageProperty
    {
        size_t width         = 0;
        size_t height        = 0;
        size_t row_pitch     = 0;
        size_t slice_pitch   = 0;
        size_t memory_offset = 0;
    };

    //-------------------------------------------------------------------------
    // initialize
    //-------------------------------------------------------------------------
public:
    /*!
     * 2Dテクスチャーを初期化します
     * @param[in] arg 初期化引数
     */
    bool initialize(const InitializeArg& arg);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 横幅
    size_t getWidth() const { return mMetadata.width; }
    //! 高さ
    size_t getHeight() const { return mMetadata.height; }
    //! 深さ or アレイサイズ
    size_t getDepth() const { return mMetadata.depth; }
    //! ミップレベル数
    size_t getMipLevels() const { return mMetadata.mip_levels; }
    //! メモリーサイズ
    size_t getMemorySize() const { return mMetadata.memory_size; }
    //! フォーマット
    gfx::TextureFormat getFormat() const { return mMetadata.format; }
    //! ディメンジョン
    gfx::TextureDimension getDimension() const { return mMetadata.dimension; }
    //! コンポーネントマッピング
    uint32_t getComponentMapping() const { return mMetadata.component_mapping; }

    /*!
     * 指定したミップレベルの画像を取得
     * @param[in] level レベル
     */
    const ImageProperty& getImageProperty(size_t level) const { return mImageProperties[level]; }

    /*!
     * 画像プロパティの先頭アドレスを取得
     * @param[in] offset メモリーオフセット
     */
    uint8_t*       getMemoryPtr(size_t offset)       { return mMemory.get() + offset; }
    const uint8_t* getMemoryPtr(size_t offset) const { return mMemory.get() + offset; }

     //-------------------------------------------------------------------------
private:
    //! メタデータ
    struct Metadata
    {
        size_t   width             = 0;
        size_t   height            = 0;
        size_t   depth             = 0;
        size_t   mip_levels        = 0;
        size_t   memory_size       = 0;
        uint32_t component_mapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
        gfx::TextureFormat    format    = gfx::TextureFormat::cUnknown;
        gfx::TextureDimension dimension = gfx::TextureDimension::c2D;
    };

    //-------------------------------------------------------------------------
private:
    Metadata                         mMetadata;
    std::unique_ptr<uint8_t[]>       mMemory;
    std::unique_ptr<ImageProperty[]> mImageProperties;

    //-------------------------------------------------------------------------
    // internal
    //-------------------------------------------------------------------------
private:
    /*!
     * 画像のピクセルサイズを計算する
     */
    size_t computePixelSize_() const;

    /*!
     * １行分のサイズと１枚分のサイズを計算する
     * @param[in] width  横幅
     * @param[in] height 高さ
     * @return <0> row_pitch
     * @return <1> slice_pitch
     */
    std::pair<size_t, size_t> computePitch_(size_t width, size_t height) const;
};
//-----------------------------------------------------------------------------
}
