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
// load
//-----------------------------------------------------------------------------
Resource Loader::loadSync(const std::string& filepath)
{
    // ファイル読み込み
    std::ifstream stream(filepath.c_str(), std::ios::binary);
    if (!stream)
    {
        BEL_ERROR("ファイルが見つかりませんでした. [%s]\n", filepath.c_str());
        return Resource();
    }

    // ファイルサイズを取得
    stream.seekg(0, std::ios::end);
    std::streampos size = stream.tellg();
    if (size < 0)
    {
        BEL_ERROR("ファイルサイズが取得できませんでした. [%s]\n", filepath.c_str());
        return Resource();
    }
    stream.seekg(0, std::ios::beg);

    // ファイルサイズが 0 ならここで終了
    if (size == 0)
    {
        return Resource(nullptr, 0);
    }

    // メモリー確保
    std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(size);
    if (!buffer)
    {
        BEL_ERROR("メモリー不足でファイル読み込みに失敗しました. [%s]\n", filepath.c_str());
        return Resource();
    }

    // ファイルを読み込んで閉じる
    stream.read(reinterpret_cast<char*>(buffer.get()), size);
    stream.close();

    // リソース生成
    return Resource(std::move(buffer), static_cast<size_t>(size));
}
//-----------------------------------------------------------------------------
// write
//-----------------------------------------------------------------------------
bool Loader::writeSync(const std::string& filepath, const Resource& resource)
{
    // 有効チェック
    if (!resource.isValid())
    {
        BEL_ERROR("無効なリソースを出力しようとしたため中止しました. [%s]\n", filepath.c_str());
        return false;
    }

    // ファイルを開く
    std::ofstream stream(filepath.c_str(), std::ios::binary);
    if (!stream)
    {
        BEL_ERROR("ファイルを開けませんでした. [%s]\n", filepath.c_str());
        return false;
    }

    // ファイル書き出し
    stream.write(reinterpret_cast<const char*>(resource.getBuffer()), resource.getSize());

    // 閉じる
    stream.close();

    return true;
}
//-----------------------------------------------------------------------------
} // bel::res::
