/*!
 * @file   belResourceTexture.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
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
        uint8_t magic[4] = { 'B', 'T', 'E', 'X'};
    };

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
