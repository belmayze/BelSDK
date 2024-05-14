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
    Vector3() {}
    //! すべて同じ数値で初期化するコンストラクター
    Vector3(float f) : mX(f), mY(f), mZ(f) {}
    //! 任意の数値で初期化するコンストラクター
    Vector3(const Vector2& v, float z);
    Vector3(float x, float y, float z) : mX(x), mY(y), mZ(z) {}
    //! 配列で初期化するコンストラクター
    Vector3(float* p) : mX(p[0]), mY(p[1]), mZ(p[2]) {}

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

    //! Z
    float&       z()       { return mZ; }
    const float& z() const { return mZ; }

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
    inline Vector3& operator += (const Vector3& rhs);
    inline Vector3& operator -= (const Vector3& rhs);
    inline Vector3& operator *= (float rhs);
    inline Vector3& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // unary operators
    //-------------------------------------------------------------------------
public:
    inline Vector3 operator + () const;
    inline Vector3 operator - () const;

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline Vector3 operator + (const Vector3& rhs) const;
    inline Vector3 operator - (const Vector3& rhs) const;
    inline Vector3 operator * (float rhs) const;
    inline Vector3 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline bool operator == (const Vector3& rhs) const;
    inline bool operator != (const Vector3& rhs) const;

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
static bel::Vector3 operator * (float f, const bel::Vector3& rhs);

#include "math/belVector3.inl.h"
