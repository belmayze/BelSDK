/*!
 * @file   main.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "platform/belPlatform.h"

/*!
 * エントリーポイント
 */
int belMain(int argc, const char** argv)
{
    // プラットフォーム
    bel::Platform::GetInstance().createWindow("BelmayzeTest");
    bel::Platform::GetInstance().enterApplicationLoop();

    return 0;
}
