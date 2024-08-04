/*!
 * @file   belRadian.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel { class Degree; }

namespace bel {
//-----------------------------------------------------------------------------
class Radian
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 初期化
    constexpr Radian(float radian) : mRadian(radian) {}
    //! Degree -> Radian
    constexpr Radian(const Degree& degree);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! value
    constexpr float getValue() const { return mRadian; }

    //-------------------------------------------------------------------------
    // operator
    //-------------------------------------------------------------------------
public:
    inline constexpr Radian& operator *= (float rhs)
    {
        mRadian *= rhs;
        return *this;
    }
    inline constexpr Radian& operator /= (float rhs)
    {
        mRadian /= rhs;
        return *this;
    }

    inline constexpr Radian& operator += (const Radian& rhs)
    {
        mRadian += rhs.mRadian;
        return *this;
    }
    inline constexpr Radian& operator -= (const Radian& rhs)
    {
        mRadian -= rhs.mRadian;
        return *this;
    }

    inline constexpr Radian operator * (float rhs) const
    {
        return Radian(mRadian * rhs);
    }
    inline constexpr Radian operator / (float rhs) const
    {
        return Radian(mRadian / rhs);
    }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    constexpr operator const float& () const { return mRadian; }

    //-------------------------------------------------------------------------
private:
    float mRadian = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::
