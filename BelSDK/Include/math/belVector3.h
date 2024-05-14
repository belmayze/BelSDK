/*!
 * @file   belVector3.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel { class Vector2; }

namespace bel {
//-----------------------------------------------------------------------------
class Vector3
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    constexpr Vector3() {}
    //! すべて同じ数値で初期化するコンストラクター
    constexpr Vector3(float f) : mX(f), mY(f), mZ(f) {}
    //! 任意の数値で初期化するコンストラクター
    constexpr Vector3(const Vector2& v, float z);
    constexpr Vector3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}
    //! 配列で初期化するコンストラクター
    constexpr Vector3(float* p) : mX(p[0]), mY(p[1]), mZ(p[2]) {}

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

    //! Z
    constexpr float&       z()       { return mZ; }
    constexpr const float& z() const { return mZ; }

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
    inline constexpr Vector3& operator += (const Vector3& rhs);
    inline constexpr Vector3& operator -= (const Vector3& rhs);
    inline constexpr Vector3& operator *= (float rhs);
    inline constexpr Vector3& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // unary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector3 operator + () const;
    inline constexpr Vector3 operator - () const;

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector3 operator + (const Vector3& rhs) const;
    inline constexpr Vector3 operator - (const Vector3& rhs) const;
    inline constexpr Vector3 operator * (float rhs) const;
    inline constexpr Vector3 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Vector3& rhs) const;
    inline constexpr bool operator != (const Vector3& rhs) const;

    //-------------------------------------------------------------------------
private:
    float mX = 0.f;
    float mY = 0.f;
    float mZ = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Vector3 operator * (float f, const bel::Vector3& rhs);

#include "math/belVector3.inl.h"
