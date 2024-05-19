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
    // request
    //-------------------------------------------------------------------------
public:
    /*!
     * ファイル読み込み
     * @param[in] filepath ファイルパス
     */
    std::unique_ptr<Resource> loadSync(const std::string& filepath);
};
//-----------------------------------------------------------------------------
} // bel::res::
