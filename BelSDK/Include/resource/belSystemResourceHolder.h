/*!
 * @file   belSystemResourceHolder.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "graphics/common/belGraphicsTexture.h"
#include "resource/belResourceShaderResource.h"
#include "resource/belResourceTexture.h"

namespace bel::res {

//-----------------------------------------------------------------------------
// システムで使用するリソース
//-----------------------------------------------------------------------------
class SystemResourceHolder
{
    //-------------------------------------------------------------------------
    // load
    //-------------------------------------------------------------------------
public:
    /*!
     * システムリソースを読み込みます
     * @param[in] filepath ファイルパス
     */
    bool loadSync(const std::string& filepath);

    /*!
     * グラフィックスリソースを生成します
     * @note リソース読み込み後、グラフィックスデバイス生成後に行います
     */
    bool createGraphicsResource();

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! デバッグ文字描画シェーダーリソース
    const ShaderResource& getDebugTextShaderResource() const { return mDebugTextShaderRes; }
    //! デバッグ文字描画テクスチャー
    const gfx::Texture& getDebugTextTexture() const { return mDebugTextTexture; }

    //-------------------------------------------------------------------------
private:
    ShaderResource mDebugTextShaderRes;
    Texture        mDebugTextTextureRes;

    gfx::Texture mDebugTextTexture;
};

//-----------------------------------------------------------------------------
} // bel::res::
