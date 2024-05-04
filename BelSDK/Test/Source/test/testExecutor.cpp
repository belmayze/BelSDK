/*!
 * @file   testRingBuffer.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "test/container/testDeque.h"
#include "test/container/testVector.h"
#include "test/testExecutor.h"

namespace app::test {
//-----------------------------------------------------------------------------
void Executor::execute()
{
    BEL_PRINT("=============================================\n");
    BEL_PRINT("テストを実行します\n");
    {
        BEL_PRINT("--------------------\n");
        BEL_PRINT("[Container] deque\n");
        Deque().test();

        BEL_PRINT("--------------------\n");
        BEL_PRINT("[Container] vector\n");
        Vector().test();
    }
    BEL_PRINT("テストを終了します\n");
    BEL_PRINT("=============================================\n");
}
//-----------------------------------------------------------------------------
}
