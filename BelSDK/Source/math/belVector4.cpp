﻿/*!
 * @file   belVector4.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "math/belVector2.h"
#include "math/belVector3.h"
#include "math/belVector4.h"

namespace bel {
//-----------------------------------------------------------------------------
// constructor
//-----------------------------------------------------------------------------
constexpr Vector4::Vector4(const Vector2& v, float z, float w)
    : mX(v.x()), mY(v.y()), mZ(z), mW(w) {}
//-----------------------------------------------------------------------------
constexpr Vector4::Vector4(const Vector3& v, float w)
    : mX(v.x()), mY(v.y()), mZ(v.z()), mW(w) {}
//-----------------------------------------------------------------------------
} // bel
