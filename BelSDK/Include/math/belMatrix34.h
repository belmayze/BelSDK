/*!
 * @file   belMatrix34.h
 * @brief  3x4行列（3行4列行列）
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "math/belVector4.h"

namespace bel {
//-----------------------------------------------------------------------------
class Matrix34
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    constexpr Matrix34() {}
    //! 直接初期化するコンストラクター
    constexpr Matrix34(const Vector4& v0, const Vector4& v1, const Vector4& v2)
        : mV{ v0, v1, v2 } {}
    //! 直接初期化するコンストラクター
    constexpr Matrix34(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23)
        : mV{ (m00, m01, m02, m03), (m10, m11, m12, m13), (m20, m21, m22, m23) } {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! at
    constexpr float&       at(uint32_t i)       { return static_cast<float*>(mV[0])[i]; }
    constexpr const float& at(uint32_t i) const { return static_cast<const float*>(mV[0])[i]; }

    //! Matrix
    constexpr float&       m(uint32_t i, uint32_t j)       { return at(i * 4 + j); }
    constexpr const float& m(uint32_t i, uint32_t j) const { return at(i * 4 + j); }

    //! row
    constexpr Vector4&       row(uint32_t i)       { return mV[i]; }
    constexpr const Vector4& row(uint32_t i) const { return mV[i]; }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    //! 配列キャスト
    constexpr operator       float* ()       { return mV[0]; }
    constexpr operator const float* () const { return mV[0]; }

    //-------------------------------------------------------------------------
    // assignment operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Matrix34& operator *= (float rhs);
    inline constexpr Matrix34& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Matrix34 operator * (float rhs) const;
    inline constexpr Matrix34 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Matrix34& rhs) const;
    inline constexpr bool operator != (const Matrix34& rhs) const;

    //-------------------------------------------------------------------------
private:
    Vector4 mV[3];
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Matrix34 operator * (float f, const bel::Matrix34& rhs);

#include "math/belMatrix34.inl.h"
