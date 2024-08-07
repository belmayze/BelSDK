﻿/*!
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

    //! at
    constexpr float&       at(uint32_t index)       { BEL_ASSERT(index < 3); return (&mX)[index]; }
    constexpr const float& at(uint32_t index) const { BEL_ASSERT(index < 3); return (&mX)[index]; }

    //!
    constexpr float&       operator[](size_t i)       { BEL_ASSERT(i < 3); return at(static_cast<uint32_t>(i)); }
    constexpr const float& operator[](size_t i) const { BEL_ASSERT(i < 3); return at(static_cast<uint32_t>(i)); }

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
    // setter
    //-------------------------------------------------------------------------
public:
    //! 与えられたベクトルを正規化してセットする
    //! @return 元のベクトルの長さを返す
    inline float setNormalize(const Vector3& v);

    //-------------------------------------------------------------------------
    // functions
    //-------------------------------------------------------------------------
public:
    //! 大きさ
    inline float length() const;

    //! 大きさの２乗
    inline constexpr float lengthSq() const;

    //! 内積
    inline constexpr float dot(const Vector3& v) const;

    //! 外積
    inline constexpr Vector3 cross(const Vector3& v) const;

    //! 正規化
    inline Vector3 normalize() const;

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
