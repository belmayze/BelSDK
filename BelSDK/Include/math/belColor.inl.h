/*!
 * @file   belColor.inl.h
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
constexpr Color& Color::operator *= (float rhs)
{
    mR *= rhs;
    mG *= rhs;
    mB *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Color& Color::operator /= (float rhs)
{
    float f_inv = 1.f / rhs;
    mR *= f_inv;
    mG *= f_inv;
    mB *= f_inv;
    return *this;
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Color Color::operator * (float rhs) const
{
    return Color(
        mR * rhs,
        mG * rhs,
        mB * rhs,
        mA
    );
}
//-----------------------------------------------------------------------------
constexpr Color Color::operator / (float rhs) const
{
    float f_inv = 1.f / rhs;
    return Color(
        mR * f_inv,
        mG * f_inv,
        mB * f_inv,
        mA
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Color::operator == (const Color& rhs) const
{
    return mR == rhs.mR && mG == rhs.mG && mB == rhs.mB && mA == rhs.mA;
}
//-----------------------------------------------------------------------------
constexpr bool Color::operator != (const Color& rhs) const
{
    return mR != rhs.mR || mG != rhs.mG || mB != rhs.mB || mA != rhs.mA;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Color operator * (float f, const bel::Color& rhs)
{
    return bel::Color(
        f * rhs.r(),
        f * rhs.g(),
        f * rhs.b(),
        rhs.a()
    );
}
