/*!
 * @file   belHidModule.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "hid/belController.h"
#include "hid/belHidModule.h"

namespace bel::hid {
//-----------------------------------------------------------------------------
// ctor / dtor
//-----------------------------------------------------------------------------
HidModule::HidModule() {}
//-----------------------------------------------------------------------------
HidModule::~HidModule() {}
//-----------------------------------------------------------------------------
// override
//-----------------------------------------------------------------------------
bool HidModule::initialize()
{
    // コントローラーの初期化
    mControllers = std::make_unique<Controller[]>(cMaxController);

    return true;
}
//-----------------------------------------------------------------------------
void HidModule::update()
{
    // コントローラーの更新
    for (uint32_t i_ctrl = 0; i_ctrl < cMaxController; ++i_ctrl)
    {
        mControllers[i_ctrl].update(i_ctrl);
    }
}
//-----------------------------------------------------------------------------
} // bel::hid::
