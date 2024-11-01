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
    __m256 t0 = _mm256_castps128_ps256(mV[0]);
    t0 = _mm256_insertf128_ps(t0, mV[1], 1); // m00, m01, m02, m03, m10, m11, m12, m13
    __m256 t1 = _mm256_castps128_ps256(mV[2]);
    t1 = _mm256_insertf128_ps(t1, mV[3], 1); // m20, m21, m22, m23, m30, m31, m32, m33

    __m256 u0 = _mm256_castps128_ps256(m.mV[0]);
    u0 = _mm256_insertf128_ps(u0, m.mV[1], 1); // m00, m01, m02, m03, m10, m11, m12, m13
    __m256 u1 = _mm256_castps128_ps256(m.mV[2]);
    u1 = _mm256_insertf128_ps(u1, m.mV[3], 1); // m20, m21, m22, m23, m30, m31, m32, m33

    __m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0)); // m00, m00, m00, m00, m10, m10, m10, m10
    __m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0)); // m20, m20, m20, m20, m30, m30, m30, m30
    __m256 b  = _mm256_permute2f128_ps(u0, u0, 0x00); // m00, m01, m02, m03, m00, m01, m02, m03
    __m256 c0 = _mm256_mul_ps(a0, b); // m00 * m00~m03, m10 * m00~m03
    __m256 c1 = _mm256_mul_ps(a1, b); // m20 * m00~m03, m30 * m00~m03

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1)); // m01, m01, m01, m01, m11, m11, m11, m11
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1)); // m21, m21, m21, m21, m31, m31, m31, m31
    b  = _mm256_permute2f128_ps(u0, u0, 0x11); // m10, m11, m12, m13, m10, m11, m12, m13
    c0 = _mm256_fmadd_ps(a0, b, c0); // m01 * m10~m13, m11 * m10~m13 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m21 * m10~m13, m31 * m10~m13 + c1

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2)); // m02, m02, m02, m02, m12, m12, m12, m12
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2)); // m22, m22, m22, m22, m32, m32, m32, m32
    b  = _mm256_permute2f128_ps(u1, u1, 0x00); // m20, m21, m22, m23, m20, m21, m22, m23
    c0 = _mm256_fmadd_ps(a0, b, c0); // m02 * m20~m23, m12 * m20~m23 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m22 * m20~m23, m32 * m20~m23 + c1

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3)); // m03, m03, m03, m03, m13, m13, m13, m13
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3)); // m23, m23, m23, m23, m33, m33, m33, m33
    b  = _mm256_permute2f128_ps(u1, u1, 0x11); // m30, m31, m32, m33, m30, m31, m32, m33
    c0 = _mm256_fmadd_ps(a0, b, c0); // m03 * m30~m33, m13 * m30~m33 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m23 * m30~m33, m33 * m30~m33 + c1

    out.mV[0] = _mm256_castps256_ps128(c0);
    out.mV[1] = _mm256_extractf128_ps(c0, 1);
    out.mV[2] = _mm256_castps256_ps128(c1);
    out.mV[3] = _mm256_extractf128_ps(c1, 1);
#   else
    Matrix44 temp;
    temp.mV[0][0] = mV[0][0] * m.mV[0][0] + mV[0][1] * m.mV[1][0] + mV[0][2] * m.mV[2][0] + mV[0][3] * m.mV[3][0];
    temp.mV[0][1] = mV[0][0] * m.mV[0][1] + mV[0][1] * m.mV[1][1] + mV[0][2] * m.mV[2][1] + mV[0][3] * m.mV[3][1];
    temp.mV[0][2] = mV[0][0] * m.mV[0][2] + mV[0][1] * m.mV[1][2] + mV[0][2] * m.mV[2][2] + mV[0][3] * m.mV[3][2];
    temp.mV[0][3] = mV[0][0] * m.mV[0][3] + mV[0][1] * m.mV[1][3] + mV[0][2] * m.mV[2][3] + mV[0][3] * m.mV[3][3];

    temp.mV[1][0] = mV[1][0] * m.mV[0][0] + mV[1][1] * m.mV[1][0] + mV[1][2] * m.mV[2][0] + mV[1][3] * m.mV[3][0];
    temp.mV[1][1] = mV[1][0] * m.mV[0][1] + mV[1][1] * m.mV[1][1] + mV[1][2] * m.mV[2][1] + mV[1][3] * m.mV[3][1];
    temp.mV[1][2] = mV[1][0] * m.mV[0][2] + mV[1][1] * m.mV[1][2] + mV[1][2] * m.mV[2][2] + mV[1][3] * m.mV[3][2];
    temp.mV[1][3] = mV[1][0] * m.mV[0][3] + mV[1][1] * m.mV[1][3] + mV[1][2] * m.mV[2][3] + mV[1][3] * m.mV[3][3];

    temp.mV[2][0] = mV[2][0] * m.mV[0][0] + mV[2][1] * m.mV[1][0] + mV[2][2] * m.mV[2][0] + mV[2][3] * m.mV[3][0];
    temp.mV[2][1] = mV[2][0] * m.mV[0][1] + mV[2][1] * m.mV[1][1] + mV[2][2] * m.mV[2][1] + mV[2][3] * m.mV[3][1];
    temp.mV[2][2] = mV[2][0] * m.mV[0][2] + mV[2][1] * m.mV[1][2] + mV[2][2] * m.mV[2][2] + mV[2][3] * m.mV[3][2];
    temp.mV[2][3] = mV[2][0] * m.mV[0][3] + mV[2][1] * m.mV[1][3] + mV[2][2] * m.mV[2][3] + mV[2][3] * m.mV[3][3];

    temp.mV[3][0] = mV[3][0] * m.mV[0][0] + mV[3][1] * m.mV[1][0] + mV[3][2] * m.mV[2][0] + mV[3][3] * m.mV[3][0];
    temp.mV[3][1] = mV[3][0] * m.mV[0][1] + mV[3][1] * m.mV[1][1] + mV[3][2] * m.mV[2][1] + mV[3][3] * m.mV[3][1];
    temp.mV[3][2] = mV[3][0] * m.mV[0][2] + mV[3][1] * m.mV[1][2] + mV[3][2] * m.mV[2][2] + mV[3][3] * m.mV[3][2];
    temp.mV[3][3] = mV[3][0] * m.mV[0][3] + mV[3][1] * m.mV[1][3] + mV[3][2] * m.mV[2][3] + mV[3][3] * m.mV[3][3];
    out = temp;
#   endif // BEL_SIMD_USE_AVX2()
    return out;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::mul(Matrix44& out, const Matrix34& m) const
{
#   if BEL_SIMD_USE_AVX2()
    __m256 t0 = _mm256_castps128_ps256(mV[0]);
    t0 = _mm256_insertf128_ps(t0, mV[1], 1); // m00, m01, m02, m03, m10, m11, m12, m13
    __m256 t1 = _mm256_castps128_ps256(mV[2]);
    t1 = _mm256_insertf128_ps(t1, mV[3], 1); // m20, m21, m22, m23, m30, m31, m32, m33

    __m256 u0 = _mm256_castps128_ps256(m.row(0));
    u0 = _mm256_insertf128_ps(u0, m.row(1), 1); // m00, m01, m02, m03, m10, m11, m12, m13
    __m256 u1 = _mm256_castps128_ps256(m.row(2));
    u1 = _mm256_insertf128_ps(u1, _mm_set_ps(1.f, 0.f, 0.f, 0.f), 1); // m20, m21, m22, m23, 0, 0, 0, 1

    __m256 a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(0, 0, 0, 0)); // m00, m00, m00, m00, m10, m10, m10, m10
    __m256 a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(0, 0, 0, 0)); // m20, m20, m20, m20, m30, m30, m30, m30
    __m256 b  = _mm256_permute2f128_ps(u0, u0, 0x00); // m00, m01, m02, m03, m00, m01, m02, m03
    __m256 c0 = _mm256_mul_ps(a0, b); // m00 * m00~m03, m10 * m00~m03
    __m256 c1 = _mm256_mul_ps(a1, b); // m20 * m00~m03, m30 * m00~m03

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(1, 1, 1, 1)); // m01, m01, m01, m01, m11, m11, m11, m11
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(1, 1, 1, 1)); // m21, m21, m21, m21, m31, m31, m31, m31
    b  = _mm256_permute2f128_ps(u0, u0, 0x11); // m10, m11, m12, m13, m10, m11, m12, m13
    c0 = _mm256_fmadd_ps(a0, b, c0); // m01 * m10~m13, m11 * m10~m13 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m21 * m10~m13, m31 * m10~m13 + c1

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(2, 2, 2, 2)); // m02, m02, m02, m02, m12, m12, m12, m12
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(2, 2, 2, 2)); // m22, m22, m22, m22, m32, m32, m32, m32
    b  = _mm256_permute2f128_ps(u1, u1, 0x00); // m20, m21, m22, m23, m20, m21, m22, m23
    c0 = _mm256_fmadd_ps(a0, b, c0); // m02 * m20~m23, m12 * m20~m23 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m22 * m20~m23, m32 * m20~m23 + c1

    a0 = _mm256_shuffle_ps(t0, t0, _MM_SHUFFLE(3, 3, 3, 3)); // m03, m03, m03, m03, m13, m13, m13, m13
    a1 = _mm256_shuffle_ps(t1, t1, _MM_SHUFFLE(3, 3, 3, 3)); // m23, m23, m23, m23, m33, m33, m33, m33
    b  = _mm256_permute2f128_ps(u1, u1, 0x11); // m30, m31, m32, m33, m30, m31, m32, m33
    c0 = _mm256_fmadd_ps(a0, b, c0); // m03 * m30~m33, m13 * m30~m33 + c0
    c1 = _mm256_fmadd_ps(a1, b, c1); // m23 * m30~m33, m33 * m30~m33 + c1

    out.mV[0] = _mm256_castps256_ps128(c0);
    out.mV[1] = _mm256_extractf128_ps(c0, 1);
    out.mV[2] = _mm256_castps256_ps128(c1);
    out.mV[3] = _mm256_extractf128_ps(c1, 1);
#   else
    Matrix44 temp;
    temp.mV[0][0] = mV[0][0] * m.m(0, 0) + mV[0][1] * m.m(1, 0) + mV[0][2] * m.m(2, 0);
    temp.mV[0][1] = mV[0][0] * m.m(0, 1) + mV[0][1] * m.m(1, 1) + mV[0][2] * m.m(2, 1);
    temp.mV[0][2] = mV[0][0] * m.m(0, 2) + mV[0][1] * m.m(1, 2) + mV[0][2] * m.m(2, 2);
    temp.mV[0][3] = mV[0][0] * m.m(0, 3) + mV[0][1] * m.m(1, 3) + mV[0][2] * m.m(2, 3) + mV[0][3];

    temp.mV[1][0] = mV[1][0] * m.m(0, 0) + mV[1][1] * m.m(1, 0) + mV[1][2] * m.m(2, 0);
    temp.mV[1][1] = mV[1][0] * m.m(0, 1) + mV[1][1] * m.m(1, 1) + mV[1][2] * m.m(2, 1);
    temp.mV[1][2] = mV[1][0] * m.m(0, 2) + mV[1][1] * m.m(1, 2) + mV[1][2] * m.m(2, 2);
    temp.mV[1][3] = mV[1][0] * m.m(0, 3) + mV[1][1] * m.m(1, 3) + mV[1][2] * m.m(2, 3) + mV[1][3];

    temp.mV[2][0] = mV[2][0] * m.m(0, 0) + mV[2][1] * m.m(1, 0) + mV[2][2] * m.m(2, 0);
    temp.mV[2][1] = mV[2][0] * m.m(0, 1) + mV[2][1] * m.m(1, 1) + mV[2][2] * m.m(2, 1);
    temp.mV[2][2] = mV[2][0] * m.m(0, 2) + mV[2][1] * m.m(1, 2) + mV[2][2] * m.m(2, 2);
    temp.mV[2][3] = mV[2][0] * m.m(0, 3) + mV[2][1] * m.m(1, 3) + mV[2][2] * m.m(2, 3) + mV[2][3];

    temp.mV[3][0] = mV[3][0] * m.m(0, 0) + mV[3][1] * m.m(1, 0) + mV[3][2] * m.m(2, 0);
    temp.mV[3][1] = mV[3][0] * m.m(0, 1) + mV[3][1] * m.m(1, 1) + mV[3][2] * m.m(2, 1);
    temp.mV[3][2] = mV[3][0] * m.m(0, 2) + mV[3][1] * m.m(1, 2) + mV[3][2] * m.m(2, 2);
    temp.mV[3][3] = mV[3][0] * m.m(0, 3) + mV[3][1] * m.m(1, 3) + mV[3][2] * m.m(2, 3) + mV[3][3];
    out = temp;
#   endif // BEL_SIMD_USE_AVX2()
    return out;
}
//-----------------------------------------------------------------------------
Vector4& Matrix44::mul(Vector4& out, const Vector4& v) const
{
    Vector4 temp;
    temp[0] = mV[0][0] * v[0] + mV[0][1] * v[1] + mV[0][2] * v[2] + mV[0][3] * v[3];
    temp[1] = mV[1][0] * v[0] + mV[1][1] * v[1] + mV[1][2] * v[2] + mV[1][3] * v[3];
    temp[2] = mV[2][0] * v[0] + mV[2][1] * v[1] + mV[2][2] * v[2] + mV[2][3] * v[3];
    temp[3] = mV[3][0] * v[0] + mV[3][1] * v[1] + mV[3][2] * v[2] + mV[3][3] * v[3];
    out = temp;
    return out;
}
//-----------------------------------------------------------------------------
// transpose / inverse
//-----------------------------------------------------------------------------
Matrix44& Matrix44::transpose(Matrix44& out) const
{
    float temp;

    out.mV[0][0] = mV[0][0];
    out.mV[1][1] = mV[1][1];
    out.mV[2][2] = mV[2][2];
    out.mV[3][3] = mV[3][3];

    temp = mV[0][1]; out.mV[0][1] = mV[1][0]; out.mV[1][0] = temp;
    temp = mV[0][2]; out.mV[0][2] = mV[2][0]; out.mV[2][0] = temp;
    temp = mV[0][3]; out.mV[0][3] = mV[3][0]; out.mV[3][0] = temp;

    temp = mV[1][2]; out.mV[1][2] = mV[2][1]; out.mV[2][1] = temp;
    temp = mV[1][3]; out.mV[1][3] = mV[3][1]; out.mV[3][1] = temp;

    temp = mV[2][3]; out.mV[2][3] = mV[3][2]; out.mV[3][2] = temp;
    return out;
}
//-----------------------------------------------------------------------------
Matrix44& Matrix44::inverse(Matrix44& out) const
{
    float det, t[3];
    Matrix44 temp;

    // m[x][0]
    t[0] = mV[2][2] * mV[3][3] - mV[2][3] * mV[3][2];
    t[1] = mV[1][2] * mV[3][3] - mV[1][3] * mV[3][2];
    t[2] = mV[1][2] * mV[2][3] - mV[1][3] * mV[2][2];
    temp.mV[0][0] =  mV[1][1] * t[0] - mV[2][1] * t[1] + mV[3][1] * t[2];
    temp.mV[1][0] = -mV[1][0] * t[0] + mV[2][0] * t[1] - mV[3][0] * t[2];

    t[0] = mV[1][0] * mV[2][1] - mV[2][0] * mV[1][1];
    t[1] = mV[1][0] * mV[3][1] - mV[3][0] * mV[1][1];
    t[2] = mV[2][0] * mV[3][1] - mV[3][0] * mV[2][1];
    temp.mV[2][0] =  mV[3][3] * t[0] - mV[2][3] * t[1] + mV[1][3] * t[2];
    temp.mV[3][0] = -mV[3][2] * t[0] + mV[2][2] * t[1] - mV[1][2] * t[2];

    det = mV[0][0] * temp.mV[0][0] + mV[0][1] * temp.mV[1][0] + mV[0][2] * temp.mV[2][0] + mV[0][3] * temp.mV[3][0];
    if (det == 0.f)
    {
        BEL_ASSERT_MSG(0, "逆行列は存在しません");
        out = *this;
        return out;
    }

    // m[x][1]
    t[0] = mV[2][2] * mV[3][3] - mV[2][3] * mV[3][2];
    t[1] = mV[0][2] * mV[3][3] - mV[0][3] * mV[3][2];
    t[2] = mV[0][2] * mV[2][3] - mV[0][3] * mV[2][2];
    temp.mV[0][1] = -mV[0][1] * t[0] + mV[2][1] * t[1] - mV[3][1] * t[2];
    temp.mV[1][1] =  mV[0][0] * t[0] - mV[2][0] * t[1] + mV[3][0] * t[2];

    t[0] = mV[0][0] * mV[2][1] - mV[2][0] * mV[0][1];
    t[1] = mV[3][0] * mV[0][1] - mV[0][0] * mV[3][1];
    t[2] = mV[2][0] * mV[3][1] - mV[3][0] * mV[2][1];
    temp.mV[2][1] = -mV[3][3] * t[0] - mV[2][3] * t[1] - mV[0][3] * t[2];
    temp.mV[3][1] =  mV[3][2] * t[0] + mV[2][2] * t[1] + mV[0][2] * t[2];

    // m[x][2]
    t[0] = mV[1][2] * mV[3][3] - mV[1][3] * mV[3][2];
    t[1] = mV[0][2] * mV[3][3] - mV[0][3] * mV[3][2];
    t[2] = mV[0][2] * mV[1][3] - mV[0][3] * mV[1][2];
    temp.mV[0][2] =  mV[0][1] * t[0] - mV[1][1] * t[1] + mV[3][1] * t[2];
    temp.mV[1][2] = -mV[0][0] * t[0] + mV[1][0] * t[1] - mV[3][0] * t[2];

    t[0] = mV[0][0] * mV[1][1] - mV[1][0] * mV[0][1];
    t[1] = mV[3][0] * mV[0][1] - mV[0][0] * mV[3][1];
    t[2] = mV[1][0] * mV[3][1] - mV[3][0] * mV[1][1];
    temp.mV[2][2] =  mV[3][3] * t[0] + mV[1][3] * t[1] + mV[0][3] * t[2];
    temp.mV[3][2] = -mV[3][2] * t[0] - mV[1][2] * t[1] - mV[0][2] * t[2];

    // m[x][3]
    t[0] = mV[1][2] * mV[2][3] - mV[1][3] * mV[2][2];
    t[1] = mV[0][2] * mV[2][3] - mV[0][3] * mV[2][2];
    t[2] = mV[0][2] * mV[1][3] - mV[0][3] * mV[1][2];
    temp.mV[0][3] = -mV[0][1] * t[0] + mV[1][1] * t[1] - mV[2][1] * t[2];
    temp.mV[1][3] =  mV[0][0] * t[0] - mV[1][0] * t[1] + mV[2][0] * t[2];

    temp.mV[2][3] =
        -mV[0][0] * (mV[1][1] * mV[2][3] - mV[1][3] * mV[2][1]) +
         mV[1][0] * (mV[0][1] * mV[2][3] - mV[0][3] * mV[2][1]) -
         mV[2][0] * (mV[0][1] * mV[1][3] - mV[0][3] * mV[1][1]);
    temp.mV[3][3] =
        mV[0][0] * (mV[1][1] * mV[2][2] - mV[1][2] * mV[2][1]) -
        mV[1][0] * (mV[0][1] * mV[2][2] - mV[0][2] * mV[2][1]) +
        mV[2][0] * (mV[0][1] * mV[1][2] - mV[0][2] * mV[1][1]);

    det = 1.f / det;
    temp *= det;

    out = temp;
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
