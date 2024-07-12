/*!
 * @file   belVector4.inl.h
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
constexpr Vector4& Vector4::operator += (const Vector4& rhs)
{
    mX += rhs.mX;
    mY += rhs.mY;
    mZ += rhs.mZ;
    mW += rhs.mW;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector4& Vector4::operator -= (const Vector4& rhs)
{
    mX -= rhs.mX;
    mY -= rhs.mY;
    mZ -= rhs.mZ;
    mW -= rhs.mW;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector4& Vector4::operator *= (float rhs)
{
    mX *= rhs;
    mY *= rhs;
    mZ *= rhs;
    mW *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Vector4& Vector4::operator /= (float rhs)
{
    float rcp = 1.f / rhs;
    mX *= rcp;
    mY *= rcp;
    mZ *= rcp;
    mW *= rcp;
    return *this;
}
//-----------------------------------------------------------------------------
// unary operators
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator + () const
{
    return Vector4(mX, mY, mZ, mW);
}
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator - () const
{
    return Vector4(-mX, -mY, -mZ, -mW);
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator + (const Vector4& rhs) const
{
    return Vector4(
        mX + rhs.mX,
        mY + rhs.mY,
        mZ + rhs.mZ,
        mW + rhs.mW
    );
}
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator - (const Vector4& rhs) const
{
    return Vector4(
        mX - rhs.mX,
        mY - rhs.mY,
        mZ - rhs.mZ,
        mW - rhs.mW
    );
}
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator * (float rhs) const
{
    return Vector4(
        mX * rhs,
        mY * rhs,
        mZ * rhs,
        mW * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Vector4 Vector4::operator / (float rhs) const
{
    float rcp = 1.f / rhs;
    return Vector4(
        mX * rcp,
        mY * rcp,
        mZ * rcp,
        mW * rcp
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Vector4::operator == (const Vector4& rhs) const
{
    return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW;
}
//-----------------------------------------------------------------------------
constexpr bool Vector4::operator != (const Vector4& rhs) const
{
    return mX != rhs.mX || mY != rhs.mY || mZ != rhs.mZ || mW != rhs.mW;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Vector4 operator * (float f, const bel::Vector4& rhs)
{
    return bel::Vector4(
        f * rhs.x(),
        f * rhs.y(),
        f * rhs.z(),
        f * rhs.w()
    );
}
