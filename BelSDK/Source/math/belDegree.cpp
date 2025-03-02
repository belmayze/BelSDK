/*!
 * @file   belDegree.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "math/belDegree.h"
#include "math/belRadian.h"

namespace bel {
//-----------------------------------------------------------------------------
Degree::Degree(const Radian& radian)
    : mDegree(radian.getValue() * (180.f / Math::cPi)) {}
//-----------------------------------------------------------------------------
} // bel
