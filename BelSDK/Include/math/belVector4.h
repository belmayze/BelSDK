/*!
 * @file   belVector4.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel { class Vector2; }
namespace bel { class Vector3; }

namespace bel {
//-----------------------------------------------------------------------------
class Vector4
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    constexpr Vector4() {}
    //! すべて同じ数値で初期化するコンストラクター
    constexpr Vector4(float f) : mX(f), mY(f), mZ(f), mW(f) {}
    //! 任意の数値で初期化するコンストラクター
    constexpr Vector4(const Vector2& v, float z, float w);
    constexpr Vector4(const Vector3& v, float w);
    constexpr Vector4(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}
    //! 配列で初期化するコンストラクター
    constexpr Vector4(float* p) : mX(p[0]), mY(p[1]), mZ(p[2]), mW(p[3]) {}

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

    //! W
    constexpr float&       w()       { return mW; }
    constexpr const float& w() const { return mW; }

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
    inline constexpr Vector4& operator += (const Vector4& rhs);
    inline constexpr Vector4& operator -= (const Vector4& rhs);
    inline constexpr Vector4& operator *= (float rhs);
    inline constexpr Vector4& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // unary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector4 operator + () const;
    inline constexpr Vector4 operator - () const;

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Vector4 operator + (const Vector4& rhs) const;
    inline constexpr Vector4 operator - (const Vector4& rhs) const;
    inline constexpr Vector4 operator * (float rhs) const;
    inline constexpr Vector4 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Vector4& rhs) const;
    inline constexpr bool operator != (const Vector4& rhs) const;

    //-------------------------------------------------------------------------
private:
    float mX = 0.f;
    float mY = 0.f;
    float mZ = 0.f;
    float mW = 0.f;
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Vector4 operator * (float f, const bel::Vector4& rhs);

#include "math/belVector4.inl.h"
