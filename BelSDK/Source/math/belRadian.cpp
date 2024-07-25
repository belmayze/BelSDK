/*!
 * @file   belRadian.cpp
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
constexpr Radian::Radian(const Degree& degree)
    : mRadian(degree.getValue() * (Math::cPi / 180.f)) {}
//-----------------------------------------------------------------------------
} // bel
