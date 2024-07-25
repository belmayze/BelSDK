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
private:
    float mRadian = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::
