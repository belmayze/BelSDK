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
    constexpr Vector2() {}
    //! すべて同じ数値で初期化するコンストラクター
    constexpr Vector2(float f) : mX(f), mY(f) {}
    //! 任意の数値で初期化するコンストラクター
    constexpr Vector2(float x, float y) : mX(x), mY(y) {}
    //! 配列で初期化するコンストラクター
    constexpr Vector2(float* p) : mX(p[0]), mY(p[1]) {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! X
    constexpr float&       x()       { return mX; }
    constexpr const float& x() const { return mX; }

    //! Y
    constexpr float&       y()       { return mY; }
    constexpr const float& y() const { return mY; }

    //! at
    constexpr float&       at(uint32_t index)       { return (&mX)[index]; }
    constexpr const float& at(uint32_t index) const { return (&mX)[index]; }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    //! 配列キャスト
    constexpr operator       float* ()       { return &mX; }
    constexpr operator const float* () const { return &mX; }

    //-------------------------------------------------------------------------
    // assignment operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector2& operator += (const Vector2& rhs);
    inline constexpr Vector2& operator -= (const Vector2& rhs);
    inline constexpr Vector2& operator *= (float rhs);
    inline constexpr Vector2& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // unary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector2 operator + () const;
    inline constexpr Vector2 operator - () const;

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector2 operator + (const Vector2& rhs) const;
    inline constexpr Vector2 operator - (const Vector2& rhs) const;
    inline constexpr Vector2 operator * (float rhs) const;
    inline constexpr Vector2 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Vector2& rhs) const;
    inline constexpr bool operator != (const Vector2& rhs) const;

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
