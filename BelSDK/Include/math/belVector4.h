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
class alignas(16) Vector4
{
    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    constexpr Vector4() : mX(0.f), mY(0.f), mZ(0.f), mW(0.f) {}
    //! すべて同じ数値で初期化するコンストラクター
    constexpr Vector4(float f) : mX(f), mY(f), mZ(f), mW(f) {}
    //! 任意の数値で初期化するコンストラクター
    constexpr Vector4(const Vector2& v, float z, float w);
    constexpr Vector4(const Vector3& v, float w);
    constexpr Vector4(float x, float y, float z, float w) : mX(x), mY(y), mZ(z), mW(w) {}
    //! 配列で初期化するコンストラクター
    constexpr Vector4(float* p) : mX(p[0]), mY(p[1]), mZ(p[2]), mW(p[3]) {}

#   if BEL_SIMD_USE_AVX2()
    //! 128bit レジスター
    constexpr Vector4(const __m128& r) : mR(r) {}
#   endif // BEL_SIMD_USE_AVX2()

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! X
    inline constexpr float&       x()       { return mX; }
    inline constexpr const float& x() const { return mX; }

    //! Y
    inline constexpr float&       y()       { return mY; }
    inline constexpr const float& y() const { return mY; }

    //! Z
    inline constexpr float&       z()       { return mZ; }
    inline constexpr const float& z() const { return mZ; }

    //! W
    inline constexpr float&       w()       { return mW; }
    inline constexpr const float& w() const { return mW; }

    //! at
    inline constexpr float&       at(uint32_t index)       { BEL_ASSERT(index < 4); return (&mX)[index]; }
    inline constexpr const float& at(uint32_t index) const { BEL_ASSERT(index < 4); return (&mX)[index]; }

    //!
    inline constexpr float&       operator[](size_t i)       { BEL_ASSERT(i < 4); return at(static_cast<uint32_t>(i)); }
    inline constexpr const float& operator[](size_t i) const { BEL_ASSERT(i < 4); return at(static_cast<uint32_t>(i)); }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    //! 配列キャスト
    inline constexpr operator       float* ()       { return mF; }
    inline constexpr operator const float* () const { return mF; }

#   if BEL_SIMD_USE_AVX2()
    //! 128bit レジスターキャスト
    inline constexpr operator       __m128& ()       { return mR; }
    inline constexpr operator const __m128& () const { return mR; }
#   endif // BEL_SIMD_USE_AVX2()

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
    union
    {
        struct
        {
            float mX, mY, mZ, mW;
        };
        float mF[4];
#       if BEL_SIMD_USE_AVX2()
        __m128 mR;
#       endif // BEL_SIMD_USE_AVX2()
    };
};
//-----------------------------------------------------------------------------
} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Vector4 operator * (float f, const bel::Vector4& rhs);

#include "math/belVector4.inl.h"
