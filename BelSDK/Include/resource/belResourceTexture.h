/*!
 * @file   belResourceTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTextureType.h"
#include "resource/belResource.h"
#include "resource/belResourceLoader.h"

namespace bel { class Image; }

namespace bel::res {

class TextureFactory;

//-----------------------------------------------------------------------------
class Texture : public Resource
{
    friend TextureFactory;

    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Texture() {}

    //-------------------------------------------------------------------------
    // image から初期化
    //-------------------------------------------------------------------------
public:
    static Texture Create(const Image& image);

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:
    //! 横幅
    uint32_t getWidth() const { return mFileHeader.width; }

    //! 高さ
    uint32_t getHeight() const { return mFileHeader.height; }

    //! 深さ
    uint32_t getDepth() const { return mFileHeader.depth; }

    //! ミップレベル
    uint32_t getMipLevels() const { return mFileHeader.mip_levels; }

    //! ディメンジョン
    gfx::TextureDimension getDimension() const { return mFileHeader.dimension; }

    //! フォーマット
    gfx::TextureFormat getFormat() const { return mFileHeader.format; }

    //! コンポーネントマッピング
    uint32_t getComponentMapping() const { return mFileHeader.component_mapping; }

    //! 画像メモリー取得
    const void* getImageMemoryPtr() const { return getBuffer(sizeof(FileHeader)); }

    //! 画像メモリーサイズ
    size_t getImageMemorySize() const { return getSize() - sizeof(FileHeader); }

    //-------------------------------------------------------------------------
private:
    //!
    static constexpr uint8_t cMagic[4] = { 'B', 'T', 'E', 'X' };

    //! ヘッダー構造体
    struct FileHeader
    {
        uint8_t               magic[4]   = { cMagic[0], cMagic[1], cMagic[2], cMagic[3] };
        uint8_t               version    = 1;
        uint8_t               reserved   = 0;
        uint16_t              width      = 1;
        uint16_t              height     = 1;
        uint16_t              depth      = 1;
        uint8_t               mip_levels = 1;
        gfx::TextureDimension dimension  = gfx::TextureDimension::c2D;
        gfx::TextureFormat    format     = gfx::TextureFormat::cR8G8B8A8_uNorm;
        uint32_t              component_mapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    };
    static_assert(sizeof(FileHeader) == 20);

    //-------------------------------------------------------------------------
private:
    FileHeader mFileHeader;

    //-------------------------------------------------------------------------
private:
    //! ヘッダー取得関数
    const FileHeader& getHeader_() const { return mFileHeader; }

    //-------------------------------------------------------------------------
private:
    //! リソース初期化
    Texture(Resource&& resource, const FileHeader& header)
        : Resource(std::move(resource))
        , mFileHeader(header) {}
};

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
class TextureFactory
{
    //-------------------------------------------------------------------------
public:
    //! リソースを使って初期化
    static Texture Create(Resource&& resource);
};

//-----------------------------------------------------------------------------
// loadSyncAs 特殊化
template <>
Texture Loader::loadSyncAs<Texture>(const std::string& filepath);

//-----------------------------------------------------------------------------
} // bel::res::
