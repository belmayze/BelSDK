/*!
 * @file   belVector2.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
class Vector2
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    Vector2() {}
    //! すべて同じ数値で初期化するコンストラクター
    Vector2(float f) : mX(f), mY(f) {}
    //! 任意の数値で初期化するコンストラクター
    Vector2(float x, float y) : mX(x), mY(y) {}
    //! 配列で初期化するコンストラクター
    Vector2(float* p) : mX(p[0]), mY(p[1]) {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! X
    float&       x()       { return mX; }
    const float& x() const { return mX; }

    //! Y
    float&       y()       { return mY; }
    const float& y() const { return mY; }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    //! 配列キャスト
    operator       float* ()       { return &mX; }
    operator const float* () const { return &mX; }

    //-------------------------------------------------------------------------
    // assignment operators
    //-------------------------------------------------------------------------
public:
    inline Vector2& operator += (const Vector2& rhs);
    inline Vector2& operator -= (const Vector2& rhs);
    inline Vector2& operator *= (float rhs);
    inline Vector2& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // unary operators
    //-------------------------------------------------------------------------
public:
    inline Vector2 operator + () const;
    inline Vector2 operator - () const;

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline Vector2 operator + (const Vector2& rhs) const;
    inline Vector2 operator - (const Vector2& rhs) const;
    inline Vector2 operator * (float rhs) const;
    inline Vector2 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline bool operator == (const Vector2& rhs) const;
    inline bool operator != (const Vector2& rhs) const;

    //-------------------------------------------------------------------------
private:
    float mX = 0.f;
    float mY = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static bel::Vector2 operator * (float f, const bel::Vector2& rhs);

#include "math/belVector2.inl.h"
