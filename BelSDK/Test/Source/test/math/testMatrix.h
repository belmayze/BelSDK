/*!
 * @file   testMatrix.h
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
class Matrix : public ITest
{
    //-------------------------------------------------------------------------
public:
    //! テスト処理
    virtual void test() override;
};
//-----------------------------------------------------------------------------
}
