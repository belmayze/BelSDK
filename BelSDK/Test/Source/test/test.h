/*!
 * @file   testBase.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace app {
//-----------------------------------------------------------------------------
class ITest
{
    //-------------------------------------------------------------------------
public:
    //! テスト処理
    virtual void test() = 0;
};
//-----------------------------------------------------------------------------
}
