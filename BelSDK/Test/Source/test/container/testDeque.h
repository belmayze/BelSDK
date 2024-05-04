/*!
 * @file   testDeque.h
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
class Deque : public ITest
{
    //-------------------------------------------------------------------------
public:
    //! テスト処理
    virtual void test() override;
};
//-----------------------------------------------------------------------------
}
