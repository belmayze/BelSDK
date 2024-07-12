/*!
 * @file   belVector2.inl.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// assignment operators
//-----------------------------------------------------------------------------
constexpr Vector2& Vector2::operator += (const Vector2& rhs)
{
    mX += rhs.mX;
    mY += rhs.mY;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector2& Vector2::operator -= (const Vector2& rhs)
{
    mX -= rhs.mX;
    mY -= rhs.mY;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector2& Vector2::operator *= (float rhs)
{
    mX *= rhs;
    mY *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector2& Vector2::operator /= (float rhs)
{
    float rcp = 1.f / rhs;
    mX *= rcp;
    mY *= rcp;
    return *this;
}
//-----------------------------------------------------------------------------
// unary operators
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator + () const
{
    return Vector2(mX, mY);
}
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator - () const
{
    return Vector2(-mX, -mY);
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator + (const Vector2& rhs) const
{
    return Vector2(
        mX + rhs.mX,
        mY + rhs.mY
    );
}
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator - (const Vector2& rhs) const
{
    return Vector2(
        mX - rhs.mX,
        mY - rhs.mY
    );
}
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator * (float rhs) const
{
    return Vector2(
        mX * rhs,
        mY * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Vector2 Vector2::operator / (float rhs) const
{
    float rcp = 1.f / rhs;
    return Vector2(
        mX * rcp,
        mY * rcp
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Vector2::operator == (const Vector2& rhs) const
{
    return mX == rhs.mX && mY == rhs.mY;
}
//-----------------------------------------------------------------------------
constexpr bool Vector2::operator != (const Vector2& rhs) const
{
    return mX != rhs.mX || mY != rhs.mY;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
bel::Vector2 operator * (float f, const bel::Vector2& rhs)
{
    return bel::Vector2(
        f * rhs.x(),
        f * rhs.y()
    );
}
