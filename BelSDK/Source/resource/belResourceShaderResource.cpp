/*!
 * @file   belResourceShaderResource.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceShaderResource.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// resource
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
ShaderResource ShaderResourceFactory::Create(Resource&& resource)
{
    BEL_ASSERT(resource.isValid());

    // ヘッダー取得
    const ShaderResource::FileHeader* p_header = reinterpret_cast<const ShaderResource::FileHeader*>(resource.getBuffer());
    if (strcmp(reinterpret_cast<const char*>(p_header->magic), "BSHA") != 0)
    {
        // ファイル破損
        return ShaderResource();
    }

    // ヘッダーを登録してシェーダーアーカイブを構築
    return ShaderResource(std::move(resource), *p_header);
}
//-----------------------------------------------------------------------------
template <>
ShaderResource Loader::loadSyncAs<ShaderResource>(const std::string& filepath)
{
    // ファイル読み込み
    Resource resource = loadSync(filepath);
    if (!resource) { return ShaderResource(); }

    // 構築
    return ShaderResourceFactory::Create(std::move(resource));
}

} // bel::res::
