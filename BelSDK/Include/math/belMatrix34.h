/*!
 * @file   belMatrix34.h
 * @brief  3x4行列（3行4列行列）
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "math/belRadian.h"
#include "math/belVector3.h"
#include "math/belVector4.h"

namespace bel {
//-----------------------------------------------------------------------------
class Matrix34
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 単位行列で初期化するコンストラクター
    constexpr Matrix34() {}
    //! 直接初期化するコンストラクター
    constexpr Matrix34(const Vector4& v0, const Vector4& v1, const Vector4& v2)
        : mV{ v0, v1, v2 } {}
    //! 直接初期化するコンストラクター
    constexpr Matrix34(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23)
        : mV{ (m00, m01, m02, m03), (m10, m11, m12, m13), (m20, m21, m22, m23) } {}

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
    constexpr Vector3 col(uint32_t i) const { return Vector3(mV[0][i], mV[1][i], mV[2][i]); }

    //! setter
    constexpr void setRow(uint32_t i, const Vector4& v) { mV[i] = v; }
    constexpr void setCol(uint32_t i, const Vector3& v) { mV[0][i] = v[0]; mV[1][i] = v[1]; mV[2][i] = v[2]; }
    constexpr void setTranslate(const Vector3& v) { setCol(3, v); }

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
    // make
    //-------------------------------------------------------------------------
public:
    //! 単位行列を生成します
    inline Matrix34& makeIdentity();

    //! スケール行列を生成します
    inline Matrix34& makeScale(const Vector3& scale);

    //! X軸回転行列を生成します
    inline Matrix34& makeRotateX(const Radian& radian);

    //! Y軸回転行列を生成します
    inline Matrix34& makeRotateY(const Radian& radian);

    //! Z軸回転行列を生成します
    inline Matrix34& makeRotateZ(const Radian& radian);

    //! 回転行列を生成します
    //! @note Z * Y * X
    inline Matrix34& makeRotate(const Radian& x, const Radian& y, const Radian& z);

    //! 回転行列を生成します
    //! @note Z * X * Y
    inline Matrix34& makeYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll);

    //! 平行移動行列を生成します
    inline Matrix34& makeTranslate(const Vector3& translate);

    //! カメラ行列を生成します
    inline Matrix34& makeLookAtRH(const Vector3& eye_pt, const Vector3& look_at, const Vector3& up);

    //-------------------------------------------------------------------------
    // multiply
    //-------------------------------------------------------------------------
public:
    //! *this *= m
    inline Matrix34& setMul(const Matrix34& m);

    //! *this * m
    inline Matrix34& mul(Matrix34& out, const Matrix34& m) const;

    //! *this * v
    inline Vector4& mul(Vector4& out, const Vector4& v) const;

    //! *this * vec4(v, 1)
    inline Vector3& mul(Vector3& out, const Vector3& v) const;

    //! *this * vec4(v, 0)
    inline Vector3& rotate(Vector3& out, const Vector3& v) const;

    //-------------------------------------------------------------------------
private:
    Vector4 mV[3] = {
        Vector4(1.f, 0.f, 0.f, 0.f),
        Vector4(0.f, 1.f, 0.f, 0.f),
        Vector4(0.f, 0.f, 1.f, 0.f)
    };
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Matrix34 operator * (float f, const bel::Matrix34& rhs);

#include "math/belMatrix34.inl.h"
