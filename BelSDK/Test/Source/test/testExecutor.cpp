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
#   if BEL_TARGET_IS_DEBUG()
    BEL_DEBUG_LOG("=============================================\n");
    BEL_DEBUG_LOG("テストを実行します\n");
    {
        BEL_DEBUG_LOG("--------------------\n");
        BEL_DEBUG_LOG("[Container] deque\n");
        Deque().test();

        BEL_DEBUG_LOG("--------------------\n");
        BEL_DEBUG_LOG("[Container] vector\n");
        Vector().test();
    }
    BEL_DEBUG_LOG("テストを終了します\n");
    BEL_DEBUG_LOG("=============================================\n");
#   endif // BEL_TARGET_IS_DEBUG()
}
//-----------------------------------------------------------------------------
}
