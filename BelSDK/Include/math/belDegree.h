/*!
 * @file   belDegree.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel { class Radian; }

namespace bel {
//-----------------------------------------------------------------------------
class Degree
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 初期化
    constexpr Degree(float degree) : mDegree(degree) {}
    //! Radian -> Degree
    Degree(const Radian& radian);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! value
    constexpr float getValue() const { return mDegree; }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    constexpr operator const float () const;

    //-------------------------------------------------------------------------
private:
    float mDegree = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::
