/*!
 * @file   testExecutor.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "memory/belSingleton.h"

namespace app::test {
//-----------------------------------------------------------------------------
class Executor : public bel::Singleton<Executor>
{
    //-------------------------------------------------------------------------
public:
    /*!
     * テストを実行します
     */
    void execute();
};
//-----------------------------------------------------------------------------
}
