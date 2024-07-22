/*!
 * @file   belMatrix34.inl.h
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
constexpr Matrix34& Matrix34::operator *= (float rhs)
{
    mV[0] *= rhs;
    mV[1] *= rhs;
    mV[2] *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Matrix34& Matrix34::operator /= (float rhs)
{
    float rcp = 1.f / rhs;
    mV[0] *= rcp;
    mV[1] *= rcp;
    mV[2] *= rcp;
    return *this;
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Matrix34 Matrix34::operator * (float rhs) const
{
    return Matrix34(
        mV[0] * rhs,
        mV[1] * rhs,
        mV[2] * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Matrix34 Matrix34::operator / (float rhs) const
{
    float rcp = 1.f / rhs;
    return Matrix34(
        mV[0] * rcp,
        mV[1] * rcp,
        mV[2] * rcp
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Matrix34::operator == (const Matrix34& rhs) const
{
    return mV[0] == rhs.mV[0] && mV[1] == rhs.mV[1] && mV[2] == rhs.mV[2];
}
//-----------------------------------------------------------------------------
constexpr bool Matrix34::operator != (const Matrix34& rhs) const
{
    return mV[0] != rhs.mV[0] || mV[1] != rhs.mV[1] || mV[2] != rhs.mV[2];
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Matrix34 operator * (float f, const bel::Matrix34& rhs)
{
    return bel::Matrix34(
        f * rhs.row(0),
        f * rhs.row(1),
        f * rhs.row(2)
    );
}
