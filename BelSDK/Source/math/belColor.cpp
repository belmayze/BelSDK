/*!
 * @file   belColor.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "math/belColor.h"
#include "math/belVector3.h"
#include "math/belVector4.h"

namespace bel {
//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
constexpr Color::Color(const Vector3& rgb, float a)
    : mR(rgb.x()), mG(rgb.y()), mB(rgb.z()), mA(a) {}
//-----------------------------------------------------------------------------
constexpr Color::Color(const Vector4& rgba)
    : mR(rgba.x()), mG(rgba.y()), mB(rgba.z()), mA(rgba.w()) {}
//-----------------------------------------------------------------------------
} // bel
