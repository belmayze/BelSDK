/*!
 * @file   belResourceShaderArchive.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceShaderArchive.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// resource
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
ShaderArchive ShaderArchiveFactory::Create(Resource&& resource)
{
    BEL_ASSERT(resource.isValid());

    // ヘッダー取得
    const ShaderArchive::FileHeader* p_header = reinterpret_cast<const ShaderArchive::FileHeader*>(resource.getBuffer());
    if (strcmp(reinterpret_cast<const char*>(p_header->magic), "BSHA") != 0)
    {
        // ファイル破損
        return ShaderArchive();
    }

    // ヘッダーを登録してシェーダーアーカイブを構築
    return ShaderArchive(std::move(resource), *p_header);
}
//-----------------------------------------------------------------------------
template <>
ShaderArchive Loader::loadSyncAs<ShaderArchive>(const std::string& filepath)
{
    // ファイル読み込み
    Resource resource = loadSync(filepath);
    if (!resource) { return ShaderArchive(); }

    // 構築
    return ShaderArchiveFactory::Create(std::move(resource));
}

} // bel::res::
