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
    float rcp = 1.f / rhs;
    mX *= rcp;
    mY *= rcp;
    mZ *= rcp;
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
    float rcp = 1.f / rhs;
    return Vector3(
        mX * rcp,
        mY * rcp,
        mZ * rcp
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
// setter
//-----------------------------------------------------------------------------
float Vector3::setNormalize(const Vector3& v)
{
    float length = v.length();
    float rcp = 1.f / length;
    mX = v.mX * rcp;
    mY = v.mY * rcp;
    mZ = v.mZ * rcp;
    return length;
}
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
float Vector3::length() const
{
    return std::sqrt(lengthSq());
}
//-----------------------------------------------------------------------------
constexpr float Vector3::lengthSq() const
{
    return mX * mX + mY * mY + mZ * mZ;
}
//-----------------------------------------------------------------------------
constexpr float Vector3::dot(const Vector3& v) const
{
    return mX * v.mX + mY * v.mY + mZ * v.mZ;
}
//-----------------------------------------------------------------------------
constexpr Vector3 Vector3::cross(const Vector3& v) const
{
    Vector3 result;
    result.mX = mY * v.mZ - mZ * v.mY;
    result.mY = mZ * v.mX - mX * v.mZ;
    result.mZ = mX * v.mY - mY * v.mX;
    return result;
}
//-----------------------------------------------------------------------------
Vector3 Vector3::normalize() const
{
    Vector3 result;
    float rcp = 1.f / length();
    result.mX = mX * rcp;
    result.mY = mY * rcp;
    result.mZ = mZ * rcp;
    return result;
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
