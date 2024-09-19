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
private:
    //! ヘッダー構造体
    struct FileHeader
    {
        uint8_t               magic[4]   = { 'B', 'T', 'E', 'X'};
        uint8_t               version    = 1;
        uint8_t               reserved   = 0;
        uint16_t              width      = 1;
        uint16_t              height     = 1;
        uint16_t              depth      = 1;
        uint8_t               mip_levels = 1;
        gfx::TextureDimension dimension  = gfx::TextureDimension::c2D;
        gfx::TextureFormat    format     = gfx::TextureFormat::cR8G8B8A8_uNorm;
    };
    static_assert(sizeof(FileHeader) == 16);

    //-------------------------------------------------------------------------
private:
    const FileHeader* mpFileHeader = nullptr;

    //-------------------------------------------------------------------------
private:
    //! ヘッダー取得関数
    const FileHeader& getHeader_() const { BEL_ASSERT(mpFileHeader != nullptr); return *mpFileHeader; }

    //-------------------------------------------------------------------------
private:
    //! リソース初期化
    Texture(Resource&& resource, const FileHeader& header)
        : Resource(std::move(resource))
        , mpFileHeader(&header) {}
};

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
class TextureFactory
{
    //-------------------------------------------------------------------------
public:
    static Texture Create(Resource&& resource);
};

//-----------------------------------------------------------------------------
// loadSyncAs 特殊化
template <>
Texture Loader::loadSyncAs<Texture>(const std::string& filepath);

//-----------------------------------------------------------------------------
} // bel::res::
