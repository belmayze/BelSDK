/*!
 * @file   belControllerButtonType.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::hid {
//-----------------------------------------------------------------------------
enum class ButtonType : uint8_t
{
    cA,
    cB,
    cX,
    cY,
    cDPadUp,
    cDPadDown,
    cDPadLeft,
    cDPadRight,
    cStart,
    cBack,
    cL1,
    cR1,
    cL2,
    cR2,
    cL3,
    cR3,

    cNum
};
//-----------------------------------------------------------------------------
} // bel::hid::
