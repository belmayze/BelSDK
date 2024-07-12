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
    float rcp = 1.f / rhs;
    mR *= rcp;
    mG *= rcp;
    mB *= rcp;
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
    float rcp = 1.f / rhs;
    return Color(
        mR * rcp,
        mG * rcp,
        mB * rcp,
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
// convert
//-----------------------------------------------------------------------------
Color Color::convertToSRGBGamut() const
{
    Color color;

    if (mR <= 0.0031308f) { color.mR = mR * 12.92f; }
    else                  { color.mR = 1.055f * std::pow(mR, 1.f / 2.4f) - 0.055f; }
    if (mG <= 0.0031308f) { color.mG = mG * 12.92f; }
    else                  { color.mG = 1.055f * std::pow(mG, 1.f / 2.4f) - 0.055f; }
    if (mB <= 0.0031308f) { color.mB = mB * 12.92f; }
    else                  { color.mB = 1.055f * std::pow(mB, 1.f / 2.4f) - 0.055f; }
    color.mA = mA;

    return color;
}
//-----------------------------------------------------------------------------
Color Color::convertToLinearGamut() const
{
    Color color;

    if (mR <= 0.04045f) { color.mR = mR / 12.92f; }
    else                { color.mR = std::pow((mR + 0.055f) / 1.055f, 2.4f); }
    if (mG <= 0.04045f) { color.mG = mG / 12.92f; }
    else                { color.mG = std::pow((mG + 0.055f) / 1.055f, 2.4f); }
    if (mB <= 0.04045f) { color.mB = mB / 12.92f; }
    else                { color.mB = std::pow((mB + 0.055f) / 1.055f, 2.4f); }
    color.mA = mA;

    return color;
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
