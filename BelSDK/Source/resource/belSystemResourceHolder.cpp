/*!
 * @file   belSystemResourceHolder.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "resource/belResourceTexture.h"
#include "resource/belSystemResourceHolder.h"

namespace bel::res {
//-----------------------------------------------------------------------------
bool SystemResourceHolder::loadSync(const std::string& filepath)
{
    // システムリソースを読み込む
    // @note 本当はアーカイブにして一度に読み込みたい

    //! デバッグ文字描画シェーダー
    mDebugTextShaderRes = res::Loader::GetInstance().loadSyncAs<ShaderResource>("Shader/DebugText.bsh");
    if (!mDebugTextShaderRes.isValid())
    {
        return false;
    }

    //! デバッグ文字描画テクスチャー
    mDebugTextTextureRes = res::Loader::GetInstance().loadSyncAs<Texture>("Texture/jis_font.btex");
    if (!mDebugTextTextureRes.isValid())
    {
        return false;
    }

    return true;
}
//-----------------------------------------------------------------------------
bool SystemResourceHolder::createGraphicsResource()
{
    //! デバッグ文字描画テクスチャー
    if (!mDebugTextTexture.initialize(mDebugTextTextureRes)) { return false; }

    return true;
}
//-----------------------------------------------------------------------------
} // bel::res::
