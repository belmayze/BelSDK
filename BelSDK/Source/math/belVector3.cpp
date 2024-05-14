/*!
 * @file   belVector3.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "math/belVector2.h"
#include "math/belVector3.h"

namespace bel {
//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
constexpr Vector3::Vector3(const Vector2& v, float z)
    : mX(v.x()), mY(v.y()), mZ(z) {}
//-----------------------------------------------------------------------------
} // bel
