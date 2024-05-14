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
Vector2& Vector2::operator += (const Vector2& rhs)
{
    mX += rhs.mX;
    mY += rhs.mY;
    return *this;
}
//-----------------------------------------------------------------------------
Vector2& Vector2::operator -= (const Vector2& rhs)
{
    mX -= rhs.mX;
    mY -= rhs.mY;
    return *this;
}
//-----------------------------------------------------------------------------
Vector2& Vector2::operator *= (float rhs)
{
    mX *= rhs;
    mY *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
Vector2& Vector2::operator /= (float rhs)
{
    float f_inv = 1.f / rhs;
    mX *= f_inv;
    mY *= f_inv;
    return *this;
}
//-----------------------------------------------------------------------------
// unary operators
//-----------------------------------------------------------------------------
Vector2 Vector2::operator + () const
{
    return Vector2(mX, mY);
}
//-----------------------------------------------------------------------------
Vector2 Vector2::operator - () const
{
    return Vector2(-mX, -mY);
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
Vector2 Vector2::operator + (const Vector2& rhs) const
{
    return Vector2(
        mX + rhs.mX,
        mY + rhs.mY
    );
}
//-----------------------------------------------------------------------------
Vector2 Vector2::operator - (const Vector2& rhs) const
{
    return Vector2(
        mX - rhs.mX,
        mY - rhs.mY
    );
}
//-----------------------------------------------------------------------------
Vector2 Vector2::operator * (float rhs) const
{
    return Vector2(
        mX * rhs,
        mY * rhs
    );
}
//-----------------------------------------------------------------------------
Vector2 Vector2::operator / (float rhs) const
{
    float f_inv = 1.f / rhs;
    return Vector2(
        mX * f_inv,
        mY * f_inv
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
bool Vector2::operator == (const Vector2& rhs) const
{
    return mX == rhs.mX && mY == rhs.mY;
}
//-----------------------------------------------------------------------------
bool Vector2::operator != (const Vector2& rhs) const
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
