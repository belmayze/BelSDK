/*!
 * @file   belMatrix44.inl.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
// assignment operators
//-----------------------------------------------------------------------------
constexpr Matrix44& Matrix44::operator *= (float rhs)
{
    mV[0] *= rhs;
    mV[1] *= rhs;
    mV[2] *= rhs;
    mV[3] *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Matrix44& Matrix44::operator /= (float rhs)
{
    float rcp = 1.f / rhs;
    mV[0] *= rcp;
    mV[1] *= rcp;
    mV[2] *= rcp;
    mV[3] *= rcp;
    return *this;
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Matrix44 Matrix44::operator * (float rhs) const
{
    return Matrix44(
        mV[0] * rhs,
        mV[1] * rhs,
        mV[2] * rhs,
        mV[3] * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Matrix44 Matrix44::operator / (float rhs) const
{
    float rcp = 1.f / rhs;
    return Matrix44(
        mV[0] * rcp,
        mV[1] * rcp,
        mV[2] * rcp,
        mV[3] * rcp
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Matrix44::operator == (const Matrix44& rhs) const
{
    return mV[0] == rhs.mV[0] && mV[1] == rhs.mV[1] && mV[2] == rhs.mV[2] && mV[3] == rhs.mV[3];
}
//-----------------------------------------------------------------------------
constexpr bool Matrix44::operator != (const Matrix44& rhs) const
{
    return mV[0] != rhs.mV[0] || mV[1] != rhs.mV[1] || mV[2] != rhs.mV[2] || mV[3] != rhs.mV[3];
}
//-----------------------------------------------------------------------------
// make
//-----------------------------------------------------------------------------
Matrix44& Matrix44::makeIdentity()
{
    mV[0][0] = 1.f; mV[0][1] = 0.f; mV[0][2] = 0.f; mV[0][3] = 0.f;
    mV[1][0] = 0.f; mV[1][1] = 1.f; mV[1][2] = 0.f; mV[1][3] = 0.f;
    mV[2][0] = 0.f; mV[2][1] = 0.f; mV[2][2] = 1.f; mV[2][3] = 0.f;
    mV[3][0] = 0.f; mV[3][1] = 0.f; mV[3][2] = 0.f; mV[3][3] = 1.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::makeOrthoProjectionRH(float width, float height, float z_near, float z_far)
{
    float n = 1.f / (z_far - z_near); // 奥行の長さを 1 に正規化する係数

    mV[0][0] = 2.f / width; mV[0][1] = 0.f;          mV[0][2] = 0.f; mV[0][3] = 0.f;
    mV[1][0] = 0.f;         mV[1][1] = 2.f / height; mV[1][2] = 0.f; mV[1][3] = 0.f;
    mV[2][0] = 0.f;         mV[2][1] = 0.f;          mV[2][2] = n;   mV[2][3] = -z_near * n;
    mV[3][0] = 0.f;         mV[3][1] = 0.f;          mV[3][2] = 0.f; mV[3][3] = 1.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::makePerspectiveProjectionRH(const Radian& fovy, float aspect, float z_near, float z_far)
{
    float f = 1.f / std::tan(fovy * 0.5f); // 距離 = 1 の時の Y 方向の大きさ
    float n = 1.f / (z_far - z_near);      // 奥行の長さを 1 に正規化する係数

    mV[0][0] = f / aspect; mV[0][1] = 0.f; mV[0][2] = 0.f;        mV[0][3] = 0.f;
    mV[1][0] = 0.f;        mV[1][1] = f;   mV[1][2] = 0.f;        mV[1][3] = 0.f;
    mV[2][0] = 0.f;        mV[2][1] = 0.f; mV[2][2] = -z_far * n; mV[2][3] = -z_far * z_near * n;
    mV[3][0] = 0.f;        mV[3][1] = 0.f; mV[3][2] = -1.f;       mV[3][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
// multiply
//-----------------------------------------------------------------------------
Matrix44& Matrix44::setMul(const Matrix44& m1, const Matrix44& m2)
{
    Matrix44 tmp;
    m1.mul(tmp, m2);
    *this = tmp;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::setMul(const Matrix44& m1, const Matrix34& m2)
{
    Matrix44 tmp;
    m1.mul(tmp, m2);
    *this = tmp;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::mul(Matrix44& out, const Matrix44& m) const
{
#   if BEL_SIMD_USE_AVX2()
    __m256 t0 = _mm256_castps128_ps256(mR[0]);
    t0 = _mm256_insertf128_ps(t0, mR[1], 1);
    __m256 t1 = _mm256_castps128_ps256(mR[2]);
    t1 = _mm256_insertf128_ps(t1, mR[3], 1);

    __m256 u0 = _mm256_castps128_ps256(m.mR[0]);
    u0 = _mm256_insertf128_ps(u0, m.mR[1], 1);
    __m256 u1 = _mm256_castps128_ps256(m.mR[2]);
    u1 = _mm256_insertf128_ps(u1, m.mR[3], 1);

    __m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0));
    __m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0));
    __m256 b0 = _mm256_permute2f128_ps(u0, u0, 0);
    __m256 c0 = _mm256_mul_ps(a0, b0);
    __m256 c1 = _mm256_mul_ps(a1, b0);

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1));
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1));
    b0 = _mm256_permute2f128_ps(u0, u0, 0x11);
    __m256 c2 = _mm256_fmadd_ps(a0, b0, c0);
    __m256 c3 = _mm256_fmadd_ps(a1, b0, c1);

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2));
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2));
    __m256 b1 = _mm256_permute2f128_ps(u1, u1, 0);
    __m256 c4 = _mm256_mul_ps(a0, b1);
    __m256 c5 = _mm256_mul_ps(a1, b1);

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3));
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3));
    b1 = _mm256_permute2f128_ps(u1, u1, 0x11);
    __m256 c6 = _mm256_fmadd_ps(a0, b1, c4);
    __m256 c7 = _mm256_fmadd_ps(a1, b1, c5);

    t0 = _mm256_add_ps(c2, c6);
    t1 = _mm256_add_ps(c3, c7);

    out.mR[0] = _mm256_castps256_ps128(t0);
    out.mR[1] = _mm256_extractf128_ps(t0, 1);
    out.mR[2] = _mm256_castps256_ps128(t1);
    out.mR[3] = _mm256_extractf128_ps(t1, 1);

#   else
    out.mV[0][0] = mV[0][0] * m.mV[0][0] + mV[0][1] * m.mV[1][0] + mV[0][2] * m.mV[2][0] + mV[0][3] * m.mV[3][0];
    out.mV[0][1] = mV[0][0] * m.mV[0][1] + mV[0][1] * m.mV[1][1] + mV[0][2] * m.mV[2][1] + mV[0][3] * m.mV[3][1];
    out.mV[0][2] = mV[0][0] * m.mV[0][2] + mV[0][1] * m.mV[1][2] + mV[0][2] * m.mV[2][2] + mV[0][3] * m.mV[3][2];
    out.mV[0][3] = mV[0][0] * m.mV[0][3] + mV[0][1] * m.mV[1][3] + mV[0][2] * m.mV[2][3] + mV[0][3] * m.mV[3][3];

    out.mV[1][0] = mV[1][0] * m.mV[0][0] + mV[1][1] * m.mV[1][0] + mV[1][2] * m.mV[2][0] + mV[1][3] * m.mV[3][0];
    out.mV[1][1] = mV[1][0] * m.mV[0][1] + mV[1][1] * m.mV[1][1] + mV[1][2] * m.mV[2][1] + mV[1][3] * m.mV[3][1];
    out.mV[1][2] = mV[1][0] * m.mV[0][2] + mV[1][1] * m.mV[1][2] + mV[1][2] * m.mV[2][2] + mV[1][3] * m.mV[3][2];
    out.mV[1][3] = mV[1][0] * m.mV[0][3] + mV[1][1] * m.mV[1][3] + mV[1][2] * m.mV[2][3] + mV[1][3] * m.mV[3][3];

    out.mV[2][0] = mV[2][0] * m.mV[0][0] + mV[2][1] * m.mV[1][0] + mV[2][2] * m.mV[2][0] + mV[2][3] * m.mV[3][0];
    out.mV[2][1] = mV[2][0] * m.mV[0][1] + mV[2][1] * m.mV[1][1] + mV[2][2] * m.mV[2][1] + mV[2][3] * m.mV[3][1];
    out.mV[2][2] = mV[2][0] * m.mV[0][2] + mV[2][1] * m.mV[1][2] + mV[2][2] * m.mV[2][2] + mV[2][3] * m.mV[3][2];
    out.mV[2][3] = mV[2][0] * m.mV[0][3] + mV[2][1] * m.mV[1][3] + mV[2][2] * m.mV[2][3] + mV[2][3] * m.mV[3][3];

    out.mV[3][0] = mV[3][0] * m.mV[0][0] + mV[3][1] * m.mV[1][0] + mV[3][2] * m.mV[2][0] + mV[3][3] * m.mV[3][0];
    out.mV[3][1] = mV[3][0] * m.mV[0][1] + mV[3][1] * m.mV[1][1] + mV[3][2] * m.mV[2][1] + mV[3][3] * m.mV[3][1];
    out.mV[3][2] = mV[3][0] * m.mV[0][2] + mV[3][1] * m.mV[1][2] + mV[3][2] * m.mV[2][2] + mV[3][3] * m.mV[3][2];
    out.mV[3][3] = mV[3][0] * m.mV[0][3] + mV[3][1] * m.mV[1][3] + mV[3][2] * m.mV[2][3] + mV[3][3] * m.mV[3][3];
#   endif // BEL_SIMD_USE_AVX2()
    return out;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::mul(Matrix44& out, const Matrix34& m) const
{
    out.mV[0][0] = mV[0][0] * m.m(0, 0) + mV[0][1] * m.m(1, 0) + mV[0][2] * m.m(2, 0);
    out.mV[0][1] = mV[0][0] * m.m(0, 1) + mV[0][1] * m.m(1, 1) + mV[0][2] * m.m(2, 1);
    out.mV[0][2] = mV[0][0] * m.m(0, 2) + mV[0][1] * m.m(1, 2) + mV[0][2] * m.m(2, 2);
    out.mV[0][3] = mV[0][0] * m.m(0, 3) + mV[0][1] * m.m(1, 3) + mV[0][2] * m.m(2, 3) + mV[0][3];

    out.mV[1][0] = mV[1][0] * m.m(0, 0) + mV[1][1] * m.m(1, 0) + mV[1][2] * m.m(2, 0);
    out.mV[1][1] = mV[1][0] * m.m(0, 1) + mV[1][1] * m.m(1, 1) + mV[1][2] * m.m(2, 1);
    out.mV[1][2] = mV[1][0] * m.m(0, 2) + mV[1][1] * m.m(1, 2) + mV[1][2] * m.m(2, 2);
    out.mV[1][3] = mV[1][0] * m.m(0, 3) + mV[1][1] * m.m(1, 3) + mV[1][2] * m.m(2, 3) + mV[1][3];

    out.mV[2][0] = mV[2][0] * m.m(0, 0) + mV[2][1] * m.m(1, 0) + mV[2][2] * m.m(2, 0);
    out.mV[2][1] = mV[2][0] * m.m(0, 1) + mV[2][1] * m.m(1, 1) + mV[2][2] * m.m(2, 1);
    out.mV[2][2] = mV[2][0] * m.m(0, 2) + mV[2][1] * m.m(1, 2) + mV[2][2] * m.m(2, 2);
    out.mV[2][3] = mV[2][0] * m.m(0, 3) + mV[2][1] * m.m(1, 3) + mV[2][2] * m.m(2, 3) + mV[2][3];

    out.mV[3][0] = mV[3][0] * m.m(0, 0) + mV[3][1] * m.m(1, 0) + mV[3][2] * m.m(2, 0);
    out.mV[3][1] = mV[3][0] * m.m(0, 1) + mV[3][1] * m.m(1, 1) + mV[3][2] * m.m(2, 1);
    out.mV[3][2] = mV[3][0] * m.m(0, 2) + mV[3][1] * m.m(1, 2) + mV[3][2] * m.m(2, 2);
    out.mV[3][3] = mV[3][0] * m.m(0, 3) + mV[3][1] * m.m(1, 3) + mV[3][2] * m.m(2, 3) + mV[3][3];
    return out;
}
//-----------------------------------------------------------------------------
Vector4& Matrix44::mul(Vector4& out, const Vector4& v) const
{
    out[0] = mV[0][0] * v[0] + mV[0][1] * v[1] + mV[0][2] * v[2] + mV[0][3] * v[3];
    out[1] = mV[1][0] * v[0] + mV[1][1] * v[1] + mV[1][2] * v[2] + mV[1][3] * v[3];
    out[2] = mV[2][0] * v[0] + mV[2][1] * v[1] + mV[2][2] * v[2] + mV[2][3] * v[3];
    out[3] = mV[3][0] * v[0] + mV[3][1] * v[1] + mV[3][2] * v[2] + mV[3][3] * v[3];
    return out;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Matrix44 operator * (float f, const bel::Matrix44& rhs)
{
    return bel::Matrix44(
        f * rhs.row(0),
        f * rhs.row(1),
        f * rhs.row(2),
        f * rhs.row(3)
    );
}
