/*!
 * @file   belVector3.inl.h
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
constexpr Vector3& Vector3::operator += (const Vector3& rhs)
{
    mX += rhs.mX;
    mY += rhs.mY;
    mZ += rhs.mZ;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector3& Vector3::operator -= (const Vector3& rhs)
{
    mX -= rhs.mX;
    mY -= rhs.mY;
    mZ -= rhs.mZ;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector3& Vector3::operator *= (float rhs)
{
    mX *= rhs;
    mY *= rhs;
    mZ *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector3& Vector3::operator /= (float rhs)
{
    float f_inv = 1.f / rhs;
    mX *= f_inv;
    mY *= f_inv;
    mZ *= f_inv;
    return *this;
}
//-----------------------------------------------------------------------------
// unary operators
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator + () const
{
    return Vector3(mX, mY, mZ);
}
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator - () const
{
    return Vector3(-mX, -mY, -mZ);
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator + (const Vector3& rhs) const
{
    return Vector3(
        mX + rhs.mX,
        mY + rhs.mY,
        mZ + rhs.mZ
    );
}
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator - (const Vector3& rhs) const
{
    return Vector3(
        mX - rhs.mX,
        mY - rhs.mY,
        mZ - rhs.mZ
    );
}
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator * (float rhs) const
{
    return Vector3(
        mX * rhs,
        mY * rhs,
        mZ * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::operator / (float rhs) const
{
    float f_inv = 1.f / rhs;
    return Vector3(
        mX * f_inv,
        mY * f_inv,
        mZ * f_inv
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Vector3::operator == (const Vector3& rhs) const
{
    return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ;
}
//-----------------------------------------------------------------------------
constexpr bool Vector3::operator != (const Vector3& rhs) const
{
    return mX != rhs.mX || mY != rhs.mY || mZ != rhs.mZ;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Vector3 operator * (float f, const bel::Vector3& rhs)
{
    return bel::Vector3(
        f * rhs.x(),
        f * rhs.y(),
        f * rhs.z()
    );
}
