/*!
 * @file   belResourceModel.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "resource/belResource.h"
#include "resource/belResourceLoader.h"

namespace bel::res {

class ModelFactory;

//-----------------------------------------------------------------------------
class Model : public Resource
{
    friend ModelFactory;

    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    Model() {}

    //-------------------------------------------------------------------------
    // 情報取得
    //-------------------------------------------------------------------------
public:

    //-------------------------------------------------------------------------
private:
    //!
    static constexpr uint8_t cMagic[4] = { 'B', 'M', 'D', 'L' };

    //! ヘッダー構造体
    struct FileHeader
    {
        uint8_t magic[4]  = { cMagic[0], cMagic[1], cMagic[2], cMagic[3] };
        uint8_t version   = 1;
        uint8_t reserved0 = 0;
        uint8_t reserved1 = 0;
        uint8_t reserved2 = 0;
    };
    static_assert(sizeof(FileHeader) == 8);

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
    Model(Resource&& resource, const FileHeader& header)
        : Resource(std::move(resource))
        , mFileHeader(header) {}
};

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
class ModelFactory
{
    //-------------------------------------------------------------------------
public:
    static Model Create(Resource&& resource);
};

//-----------------------------------------------------------------------------
// loadSyncAs 特殊化
template <>
Model Loader::loadSyncAs<Model>(const std::string& filepath);

//-----------------------------------------------------------------------------
} // bel::res::
