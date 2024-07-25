/*!
 * @file   belMatrix34.inl.h
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
constexpr Matrix34& Matrix34::operator *= (float rhs)
{
    mV[0] *= rhs;
    mV[1] *= rhs;
    mV[2] *= rhs;
    return *this;
}
//-----------------------------------------------------------------------------
constexpr Matrix34& Matrix34::operator /= (float rhs)
{
    float rcp = 1.f / rhs;
    mV[0] *= rcp;
    mV[1] *= rcp;
    mV[2] *= rcp;
    return *this;
}
//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr Matrix34 Matrix34::operator * (float rhs) const
{
    return Matrix34(
        mV[0] * rhs,
        mV[1] * rhs,
        mV[2] * rhs
    );
}
//-----------------------------------------------------------------------------
constexpr Matrix34 Matrix34::operator / (float rhs) const
{
    float rcp = 1.f / rhs;
    return Matrix34(
        mV[0] * rcp,
        mV[1] * rcp,
        mV[2] * rcp
    );
}
//-----------------------------------------------------------------------------
// equals operators
//-----------------------------------------------------------------------------
constexpr bool Matrix34::operator == (const Matrix34& rhs) const
{
    return mV[0] == rhs.mV[0] && mV[1] == rhs.mV[1] && mV[2] == rhs.mV[2];
}
//-----------------------------------------------------------------------------
constexpr bool Matrix34::operator != (const Matrix34& rhs) const
{
    return mV[0] != rhs.mV[0] || mV[1] != rhs.mV[1] || mV[2] != rhs.mV[2];
}
//-----------------------------------------------------------------------------
// make
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeIdentity()
{
    mV[0][0] = 1.f; mV[0][1] = 0.f; mV[0][2] = 0.f; mV[0][3] = 0.f;
    mV[1][0] = 0.f; mV[1][1] = 1.f; mV[1][2] = 0.f; mV[1][3] = 0.f;
    mV[2][0] = 0.f; mV[2][1] = 0.f; mV[2][2] = 1.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeScale(const Vector3& scale)
{
    mV[0][0] = scale.x(); mV[0][1] = 0.f;       mV[0][2] = 0.f;       mV[0][3] = 0.f;
    mV[1][0] = 0.f;       mV[1][1] = scale.y(); mV[1][2] = 0.f;       mV[1][3] = 0.f;
    mV[2][0] = 0.f;       mV[2][1] = 0.f;       mV[2][2] = scale.z(); mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeRotateX(const Radian& radian)
{
    float sin = std::sin(radian.getValue());
    float cos = std::cos(radian.getValue());
    mV[0][0] = 1.f; mV[0][1] = 0.f; mV[0][2] =  0.f;
    mV[1][0] = 0.f; mV[1][1] = cos; mV[1][2] = -sin;
    mV[2][0] = 0.f; mV[2][1] = sin; mV[2][2] =  cos;
    mV[0][3] = 0.f; mV[1][3] = 0.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeRotateY(const Radian& radian)
{
    float sin = std::sin(radian.getValue());
    float cos = std::cos(radian.getValue());
    mV[0][0] =  cos; mV[0][1] = 0.f; mV[0][2] = sin;
    mV[1][0] =  0.f; mV[1][1] = 1.f; mV[1][2] = 0.f;
    mV[2][0] = -sin; mV[2][1] = 0.f; mV[2][2] = cos;
    mV[0][3] = 0.f; mV[1][3] = 0.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeRotateZ(const Radian& radian)
{
    float sin = std::sin(radian.getValue());
    float cos = std::cos(radian.getValue());
    mV[0][0] = cos; mV[0][1] = -sin; mV[0][2] = 0.f;
    mV[1][0] = sin; mV[1][1] =  cos; mV[1][2] = 0.f;
    mV[2][0] = 0.f; mV[2][1] =  0.f; mV[2][2] = 1.f;
    mV[0][3] = 0.f; mV[1][3] = 0.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeRotate(const Radian& x, const Radian& y, const Radian& z)
{
    float sinX = std::sin(x.getValue()), sinY = std::sin(y.getValue()), sinZ = std::sin(z.getValue());
    float cosX = std::cos(x.getValue()), cosY = std::cos(y.getValue()), cosZ = std::cos(z.getValue());
    
    //         | cosZ -sinZ 0 | |  cosY 0 sinY | | 1  0     0   |
    // ZYX   = | sinZ  cosZ 0 | |   0   1  0   | | 0 cosX -sinX |
    //         |  0     0   1 | | -sinY 0 cosY | | 0 sinX  cosX |
    //
    //         | cosYcosZ -sinZ sinYcosZ | | 1  0     0   |
    // (ZY)X = | cosYsinZ  cosZ sinYsinZ | | 0 cosX -sinX |
    //         |  -sinY     0     cosY   | | 0 sinX  cosX |
    //
    //         | cosYcosZ sinXsinYcosZ-cosXsinZ cosXsinYcosZ+sinXsinZ |
    // (ZYX) = | cosYsinZ sinXsinYsinZ+cosXcosZ cosXsinYsinZ-sinXcosZ |
    //         |  -sinY           sinXcosY              cosXcosY      |

    mV[0][0] = cosY * cosZ; mV[0][1] = sinX * sinY * cosZ - cosX * sinZ; mV[0][2] = cosX * sinY * cosZ + sinX * sinZ;
    mV[1][0] = cosY * sinZ; mV[1][1] = sinX * sinY * sinZ + cosX * cosZ; mV[1][2] = cosX * sinY * sinZ - sinX * cosZ;
    mV[2][0] =       -sinY; mV[2][1] =                      sinX * cosY; mV[2][2] =                      cosX * cosY;
    mV[0][3] = 0.f; mV[1][3] = 0.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll)
{
    float sinX = std::sin(pitch.getValue()), sinY = std::sin(yaw.getValue()), sinZ = std::sin(roll.getValue());
    float cosX = std::cos(pitch.getValue()), cosY = std::cos(yaw.getValue()), cosZ = std::cos(roll.getValue());

    //         | cosZ -sinZ 0 | | 1  0     0   | |  cosY 0 sinY |
    // ZXY   = | sinZ  cosZ 0 | | 0 cosX -sinX | |   0   1  0   |
    //         |  0     0   1 | | 0 sinX  cosX | | -sinY 0 cosY |
    //
    //         | cosZ -cosXsinZ  sinXsinZ | |  cosY 0 sinY |
    // (ZX)Y = | sinZ  cosXcosZ -sinXcosZ | |   0   1  0   |
    //         |  0      sinX      cosX   | | -sinY 0 cosY |
    //
    //         | cosYcosZ-sinXsinYsinZ -cosXsinZ sinYcosZ+sinXcosYsinZ |
    // (ZXY) = | cosYsinZ+sinXsinYcosZ  cosXcosZ sinYsinZ-sinXcosYcosZ |
    //         |      -cosXsinY           sinX         cosXcosY        |
        
    mV[0][0] = cosY * cosZ - sinX * sinY * sinZ; mV[0][1] = -cosX * sinZ; mV[0][2] = sinY * cosZ + sinX * cosY * sinZ;
    mV[1][0] = cosY * sinZ + sinX * sinY * cosZ; mV[1][1] =  cosX * cosZ; mV[1][2] = sinY * sinZ - sinX * cosY * cosZ;
    mV[2][0] =                     -cosX * sinY; mV[2][1] =         sinX; mV[2][2] =                      cosX * cosY;
    mV[0][3] = 0.f; mV[1][3] = 0.f; mV[2][3] = 0.f;
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeTranslate(const Vector3& translate)
{
    mV[0][0] = 1.f; mV[0][1] = 0.f; mV[0][2] = 0.f; mV[0][3] = translate.x();
    mV[1][0] = 0.f; mV[1][1] = 1.f; mV[1][2] = 0.f; mV[1][3] = translate.y();
    mV[2][0] = 0.f; mV[2][1] = 0.f; mV[2][2] = 1.f; mV[2][3] = translate.z();
    return *this;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::makeLookAtRH(const Vector3& eye_pt, const Vector3& look_at, const Vector3& up)
{
    // カメラの姿勢ベクトルを計算する
    Vector3 x, y, z;
    z.setNormalize(eye_pt - look_at); // 視線ベクトル
    y.setNormalize(up);
    x.setNormalize(y.cross(z));
    y.setNormalize(z.cross(x)); // 上方向ベクトルを再計算

    // 行列をセットします
    // View 行列は、姿勢ベクトルは逆行列（倒置）になり、移動量は反転する
    mV[0][0] = x.x(); mV[0][1] = x.y(); mV[0][2] = x.z(); mV[0][3] = -x.dot(eye_pt);
    mV[1][0] = y.x(); mV[1][1] = y.y(); mV[1][2] = y.z(); mV[1][3] = -y.dot(eye_pt);
    mV[2][0] = z.x(); mV[2][1] = z.y(); mV[2][2] = z.z(); mV[2][3] = -z.dot(eye_pt);
    return *this;
}
//-----------------------------------------------------------------------------
// multiply
//-----------------------------------------------------------------------------
Matrix34& Matrix34::setMul(const Matrix34& m)
{
    Matrix34 tmp;
    mul(tmp, m);
    *this = tmp;
}
//-----------------------------------------------------------------------------
Matrix34& Matrix34::mul(Matrix34& out, const Matrix34& m) const
{
    out.mV[0][0] = mV[0][0] * m.mV[0][0] + mV[0][1] * m.mV[1][0] + mV[0][2] * m.mV[2][0];
    out.mV[0][1] = mV[0][0] * m.mV[0][1] + mV[0][1] * m.mV[1][1] + mV[0][2] * m.mV[2][1];
    out.mV[0][2] = mV[0][0] * m.mV[0][2] + mV[0][1] * m.mV[1][2] + mV[0][2] * m.mV[2][2];
    out.mV[0][3] = mV[0][0] * m.mV[0][3] + mV[0][1] * m.mV[1][3] + mV[0][2] * m.mV[2][3] + mV[0][3];

    out.mV[1][0] = mV[1][0] * m.mV[0][0] + mV[1][1] * m.mV[1][0] + mV[1][2] * m.mV[2][0];
    out.mV[1][1] = mV[1][0] * m.mV[0][1] + mV[1][1] * m.mV[1][1] + mV[1][2] * m.mV[2][1];
    out.mV[1][2] = mV[1][0] * m.mV[0][2] + mV[1][1] * m.mV[1][2] + mV[1][2] * m.mV[2][2];
    out.mV[1][3] = mV[1][0] * m.mV[0][3] + mV[1][1] * m.mV[1][3] + mV[1][2] * m.mV[2][3] + mV[1][3];

    out.mV[2][0] = mV[2][0] * m.mV[0][0] + mV[2][1] * m.mV[1][0] + mV[2][2] * m.mV[2][0];
    out.mV[2][1] = mV[2][0] * m.mV[0][1] + mV[2][1] * m.mV[1][1] + mV[2][2] * m.mV[2][1];
    out.mV[2][2] = mV[2][0] * m.mV[0][2] + mV[2][1] * m.mV[1][2] + mV[2][2] * m.mV[2][2];
    out.mV[2][3] = mV[2][0] * m.mV[0][3] + mV[2][1] * m.mV[1][3] + mV[2][2] * m.mV[2][3] + mV[2][3];
    return out;
}
//-----------------------------------------------------------------------------
Vector4& Matrix34::mul(Vector4& out, const Vector4& v) const
{
    out[0] = mV[0][0] * v[0] + mV[0][1] * v[1] + mV[0][2] * v[2] + mV[0][3] * v[3];
    out[1] = mV[1][0] * v[0] + mV[1][1] * v[1] + mV[1][2] * v[2] + mV[1][3] * v[3];
    out[2] = mV[2][0] * v[0] + mV[2][1] * v[1] + mV[2][2] * v[2] + mV[2][3] * v[3];
    out[3] = v[3];
    return out;
}
//-----------------------------------------------------------------------------
Vector3& Matrix34::mul(Vector3& out, const Vector3& v) const
{
    out[0] = mV[0][0] * v[0] + mV[0][1] * v[1] + mV[0][2] * v[2] + mV[0][3];
    out[1] = mV[1][0] * v[0] + mV[1][1] * v[1] + mV[1][2] * v[2] + mV[1][3];
    out[2] = mV[2][0] * v[0] + mV[2][1] * v[1] + mV[2][2] * v[2] + mV[2][3];
    return out;
}
//-----------------------------------------------------------------------------
Vector3& Matrix34::rotate(Vector3& out, const Vector3& v) const
{
    out[0] = mV[0][0] * v[0] + mV[0][1] * v[1] + mV[0][2] * v[2];
    out[1] = mV[1][0] * v[0] + mV[1][1] * v[1] + mV[1][2] * v[2];
    out[2] = mV[2][0] * v[0] + mV[2][1] * v[1] + mV[2][2] * v[2];
    return out;
}
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
constexpr bel::Matrix34 operator * (float f, const bel::Matrix34& rhs)
{
    return bel::Matrix34(
        f * rhs.row(0),
        f * rhs.row(1),
        f * rhs.row(2)
    );
}
