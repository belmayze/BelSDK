/*!
 * @file   belResourceLoader.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// std
#include <fstream>
// bel
#include "resource/belResource.h"
#include "resource/belResourceLoader.h"

namespace bel::res {
//-----------------------------------------------------------------------------
std::unique_ptr<Resource> Loader::loadSync(const std::string& filepath)
{
    // ファイル読み込み
    std::ifstream stream(filepath.c_str(), std::ios::binary);
    if (!stream)
    {
        BEL_ERROR("ファイルが見つかりませんでした. [%s]\n", filepath.c_str());
        return nullptr;
    }

    // ファイルサイズを取得
    stream.seekg(0, std::ios::end);
    std::streampos size = stream.tellg();
    if (size < 0)
    {
        BEL_ERROR("ファイルサイズが取得できませんでした. [%s]\n", filepath.c_str());
        return nullptr;
    }
    stream.seekg(0, std::ios::beg);

    // ファイルサイズが 0 ならここで終了
    if (size == 0)
    {
        return std::make_unique<Resource>(nullptr, 0);
    }

    // メモリー確保
    std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(size);
    if (!buffer)
    {
        BEL_ERROR("メモリー不足でファイル読み込みに失敗しました. [%s]\n", filepath.c_str());
        return nullptr;
    }

    // ファイルを読み込んで閉じる
    stream.read(reinterpret_cast<char*>(buffer.get()), size);
    stream.close();

    // リソース生成
    return std::make_unique<Resource>(std::move(buffer), static_cast<size_t>(size));
}
//-----------------------------------------------------------------------------
} // bel::res::
