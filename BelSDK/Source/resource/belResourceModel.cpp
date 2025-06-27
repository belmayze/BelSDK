/*!
 * @file   belResourceModel.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceModel.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
Model ModelFactory::Create(Resource&& resource)
{
    BEL_ASSERT(resource.isValid());

    // ヘッダー取得
    const Model::FileHeader* p_header = reinterpret_cast<const Model::FileHeader*>(resource.getBuffer());
    if (memcmp(p_header->magic, Model::cMagic, 4) != 0)
    {
        // ファイル破損
        return Model();
    }

    // ヘッダーを登録してシェーダーアーカイブを構築
    return Model(std::move(resource), *p_header);
}
//-----------------------------------------------------------------------------
template <>
Model Loader::loadSyncAs<Model>(const std::string& filepath)
{
    // ファイル読み込み
    Resource resource = loadSync(filepath);
    if (!resource) { return Model(); }

    // 構築
    return ModelFactory::Create(std::move(resource));
}

} // bel::res::
