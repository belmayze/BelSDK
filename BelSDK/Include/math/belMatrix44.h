/*!
 * @file   belMatrix44.h
 * @brief  4x4行列（4行4列行列）
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "math/belRadian.h"
#include "math/belMatrix34.h"
#include "math/belVector4.h"

namespace bel {
//-----------------------------------------------------------------------------
class alignas(16) Matrix44
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 単位行列で初期化するコンストラクター
    constexpr Matrix44() : mV{ {1.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 1.f} } {}
    //! 直接初期化するコンストラクター
    constexpr Matrix44(const Vector4& v0, const Vector4& v1, const Vector4& v2, const Vector4& v3)
        : mV{ v0, v1, v2, v3 } {}
    //! 直接初期化するコンストラクター
    constexpr Matrix44(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33
    ) : mV{ Vector4(m00, m01, m02, m03), Vector4(m10, m11, m12, m13), Vector4(m20, m21, m22, m23), Vector4(m30, m31, m32, m33) } {}

    //-------------------------------------------------------------------------
    // getter / setter
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

    //! col
    constexpr Vector4 col(uint32_t i) const { return Vector4(mV[0][i], mV[1][i], mV[2][i], mV[3][i]); }

    //! setter
    constexpr void setRow(uint32_t i, const Vector4& v) { mV[i] = v; }
    constexpr void setCol(uint32_t i, const Vector4& v) { mV[0][i] = v[0]; mV[1][i] = v[1]; mV[2][i] = v[2]; mV[3][i] = v[3]; }

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
    inline constexpr Matrix44& operator *= (float rhs);
    inline constexpr Matrix44& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Matrix44 operator * (float rhs) const;
    inline constexpr Matrix44 operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Matrix44& rhs) const;
    inline constexpr bool operator != (const Matrix44& rhs) const;

    //-------------------------------------------------------------------------
    // make
    //-------------------------------------------------------------------------
public:
    //! 単位行列を生成します
    inline Matrix44& makeIdentity();

    //! 平行投影プロジェクション行列を生成します
    inline Matrix44& makeOrthoProjectionRH(float width, float height, float z_near, float z_far);

    //! 透視投影プロジェクション行列を生成します
    inline Matrix44& makePerspectiveProjectionRH(const Radian& fovy, float aspect, float z_near, float z_far);

    //-------------------------------------------------------------------------
    // multiply
    //-------------------------------------------------------------------------
public:
    //! *this *= m
    inline Matrix44& setMul(const Matrix44& m1, const Matrix44& m2);

    //! *this *= m
    inline Matrix44& setMul(const Matrix44& m2, const Matrix34& m1);

    //! *this * m
    inline Matrix44& mul(Matrix44& out, const Matrix44& m) const;

    //! *this * m
    inline Matrix44& mul(Matrix44& out, const Matrix34& m) const;

    //! *this * v
    inline Vector4& mul(Vector4& out, const Vector4& v) const;

    //-------------------------------------------------------------------------
    // transpose / inverse
    //-------------------------------------------------------------------------
public:
    //! 倒置行列を求めます
    inline Matrix44& transpose(Matrix44& out) const;

    //! 逆行列を求めます
    inline Matrix44& inverse(Matrix44& out) const;

    //-------------------------------------------------------------------------
private:
    union
    {
        struct
        {
            float mM00, mM01, mM02, mM03;
            float mM10, mM11, mM12, mM13;
            float mM20, mM21, mM22, mM23;
            float mM30, mM31, mM32, mM33;
        };
        Vector4 mV[4];
        float   mF[16];
    };
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Matrix44 operator * (float f, const bel::Matrix44& rhs);

#include "math/belMatrix44.inl.h"
