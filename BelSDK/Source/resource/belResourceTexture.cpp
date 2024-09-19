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

    // データチェック
    bool failed = false;
    if (image.getWidth()     > std::numeric_limits<uint16_t>::max()) { BEL_ERROR("テクスチャーの横幅が上限を超えています"); failed = true; }
    if (image.getHeight()    > std::numeric_limits<uint16_t>::max()) { BEL_ERROR("テクスチャーの高さが上限を超えています"); failed = true; }
    if (image.getDepth()     > std::numeric_limits<uint16_t>::max()) { BEL_ERROR("テクスチャーの深さが上限を超えています"); failed = true; }
    if (image.getMipLevels() > std::numeric_limits<uint8_t>::max())  { BEL_ERROR("テクスチャーのミップ数が上限を超えています"); failed = true; }
    if (failed) { return Texture(); }

    // ヘッダー初期化
    FileHeader& header = *(new (memory.get()) FileHeader());
    header.width      = static_cast<uint16_t>(image.getWidth());
    header.height     = static_cast<uint16_t>(image.getHeight());
    header.depth      = static_cast<uint16_t>(image.getDepth());
    header.mip_levels = static_cast<uint8_t>(image.getMipLevels());
    header.format     = image.getFormat();
    header.dimension  = image.getDimension();

    // できればメモリーコピー不要にしたい
    std::memcpy(memory.get() + sizeof(FileHeader), image.getMemoryPtr(0), image.getMemorySize());

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
    if (memcmp(p_header->magic, "BTEX", 4) != 0)
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
