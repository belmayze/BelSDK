/*!
 * @file   belResourceTexture.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "image/belImage.h"
#include "resource/belResourceTexture.h"

namespace bel::res {
//-----------------------------------------------------------------------------
// resource
//-----------------------------------------------------------------------------
Texture Texture::Create(const Image& image)
{
    // ヘッダー + メモリー領域を確保
    size_t memory_size = sizeof(FileHeader) + image.getMemorySize();
    std::unique_ptr<uint8_t[]> memory = std::make_unique<uint8_t[]>(memory_size);

    // ヘッダー初期化
    FileHeader& header = *(new (memory.get()) FileHeader());

    // オブジェクト生成
    return TextureFactory::Create(Resource(std::move(memory), memory_size));
}
//-----------------------------------------------------------------------------
// factory
//-----------------------------------------------------------------------------
Texture TextureFactory::Create(Resource&& resource)
{
    BEL_ASSERT(resource.isValid());

    // ヘッダー取得
    const Texture::FileHeader* p_header = reinterpret_cast<const Texture::FileHeader*>(resource.getBuffer());
    if (strcmp(reinterpret_cast<const char*>(p_header->magic), "BTEX") != 0)
    {
        // ファイル破損
        return Texture();
    }

    // ヘッダーを登録してシェーダーアーカイブを構築
    return Texture(std::move(resource), *p_header);
}
//-----------------------------------------------------------------------------
template <>
Texture Loader::loadSyncAs<Texture>(const std::string& filepath)
{
    // ファイル読み込み
    Resource resource = loadSync(filepath);
    if (!resource) { return Texture(); }

    // 構築
    return TextureFactory::Create(std::move(resource));
}

} // bel::res::
