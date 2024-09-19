/*!
 * @file   belResourceLoader.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "memory/belSingleton.h"

namespace bel::res { class Resource; }

namespace bel::res {
//-----------------------------------------------------------------------------
class Loader : public Singleton<Loader>
{
    //-------------------------------------------------------------------------
    // load
    //-------------------------------------------------------------------------
public:
    /*!
     * ファイル読み込み
     * @param[in] filepath ファイルパス
     */
    Resource loadSync(const std::string& filepath);

    /*!
     * ファイル読み込み (cast 版)
     * @param[in] filepath ファイルパス
     */
    template <class T>
    T loadSyncAs(const std::string& filepath) { return T(); }

    //-------------------------------------------------------------------------
    // write
    //-------------------------------------------------------------------------
public:
    /*!
     * ファイルの書き出し
     * @param[in] filepath ファイルパス
     */
    bool writeSync(const std::string& filepath, const Resource& resource);
    
};
//-----------------------------------------------------------------------------
} // bel::res::
