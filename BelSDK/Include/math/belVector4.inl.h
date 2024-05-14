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
Vector4& Vector4::operator += (const Vector4& rhs)
{
    mX += rhs.mX;
    mY += rhs.mY;
    mZ += rhs.mZ;
    mW += rhs.mW;
    return *this;
}
//-----------------------------------------------------------------------------
Vector4& Vector4::operator -= (const Vector4& rhs)
{
    mX -= rhs.mX;
    mY -= rhs.mY;
    mZ -= rhs.mZ;
    mW -= rhs.mW;
    return *this;
}
//-----------------------------------------------------------------------------
Vector4& Vector4::operator *= (float rhs)
{
    mX *= rhs;
    mY *= rhs;
    mZ *= rhs;
    mW *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
Vector4& Vector4::operator /= (float rhs)
{
    float f_inv = 1.f / rhs;
    mX *= f_inv;
    mY *= f_inv;
    mZ *= f_inv;
    mW *= f_inv;
    return *this;
}
//-----------------------------------------------------------------------------
// unary operators
//-----------------------------------------------------------------------------
Vector4 Vector4::operator + () const
{
    return Vector4(mX, mY, mZ, mW);
}
//-----------------------------------------------------------------------------
Vector4 Vector4::operator - () const
{
    return Vector4(-mX, -mY, -mZ, -mW);
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
Vector4 Vector4::operator + (const Vector4& rhs) const
{
    return Vector4(
        mX + rhs.mX,
        mY + rhs.mY,
        mZ + rhs.mZ,
        mW + rhs.mW
    );
}
//-----------------------------------------------------------------------------
Vector4 Vector4::operator - (const Vector4& rhs) const
{
    return Vector4(
        mX - rhs.mX,
        mY - rhs.mY,
        mZ - rhs.mZ,
        mW - rhs.mW
    );
}
//-----------------------------------------------------------------------------
Vector4 Vector4::operator * (float rhs) const
{
    return Vector4(
        mX * rhs,
        mY * rhs,
        mZ * rhs,
        mW * rhs
    );
}
//-----------------------------------------------------------------------------
Vector4 Vector4::operator / (float rhs) const
{
    float f_inv = 1.f / rhs;
    return Vector4(
        mX * f_inv,
        mY * f_inv,
        mZ * f_inv,
        mW * f_inv
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
bool Vector4::operator == (const Vector4& rhs) const
{
    return mX == rhs.mX && mY == rhs.mY && mZ == rhs.mZ && mW == rhs.mW;
}
//-----------------------------------------------------------------------------
bool Vector4::operator != (const Vector4& rhs) const
{
    return mX != rhs.mX || mY != rhs.mY || mZ != rhs.mZ || mW != rhs.mW;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
bel::Vector4 operator * (float f, const bel::Vector4& rhs)
{
    return bel::Vector4(
        f * rhs.x(),
        f * rhs.y(),
        f * rhs.z(),
        f * rhs.w()
    );
}
