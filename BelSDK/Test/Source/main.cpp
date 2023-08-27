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
 * �G���g���[�|�C���g
 */
int belMain(int argc, const char** argv)
{
    // �v���b�g�t�H�[��
    bel::Platform::GetInstance().createWindow("BelmayzeTest");
    bel::Platform::GetInstance().enterApplicationLoop();

    return 0;
}
