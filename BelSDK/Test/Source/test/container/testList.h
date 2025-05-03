/*!
 * @file   testList.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// app
#include "test/test.h"

namespace app::test {
//-----------------------------------------------------------------------------
class List : public ITest
{
    //-------------------------------------------------------------------------
public:
    //! テスト処理
    virtual void test() override;
};
//-----------------------------------------------------------------------------
}
