/*!
 * @file   belResourceShaderArchive.h
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
//-----------------------------------------------------------------------------
class ShaderArchive : public Resource
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 無効状態で初期化
    ShaderArchive() {}

    //! リソース初期化
    ShaderArchive(std::unique_ptr<uint8_t[]>&& p_buffer, size_t size)
        : Resource(std::move(p_buffer), size) {}
};

//-----------------------------------------------------------------------------
class ShaderArchiveFactory
{
public:
    static ShaderArchive MakeResource(Resource&& resource);
};

//-----------------------------------------------------------------------------
} // bel::res::

// loadSyncAs 特殊化
namespace bel::res {

template <>
ShaderArchive Loader::loadSyncAs<ShaderArchive>(const std::string& filepath);

}
