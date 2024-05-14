/*!
 * @file   belColor.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel { class Vector3; }
namespace bel { class Vector4; }

namespace bel {
//-----------------------------------------------------------------------------
class Color
{
    //-------------------------------------------------------------------------
    // 定数
    //-------------------------------------------------------------------------
public:
    //! 黒
    static constexpr Color cBlack() { return Color(0.00f, 0.00f, 0.00f); }
    //! 18% グレー
    static constexpr Color cGray()  { return Color(0.18f, 0.18f, 0.18f); }
    //! 白
    static constexpr Color cWhite() { return Color(1.00f, 1.00f, 1.00f); }

    //-------------------------------------------------------------------------
    // constructor
    //-------------------------------------------------------------------------
public:
    //! 0で初期化するコンストラクター
    constexpr Color() {}
    //! 任意の数値で初期化するコンストラクター
    constexpr Color(const Vector3& rgb, float a = 1.f);
    constexpr Color(const Vector4& rgba);
    constexpr Color(float r, float g, float b, float a = 1.f) : mR(r), mG(g), mB(b), mA(a) {}
    //! 配列で初期化するコンストラクター
    constexpr Color(float* p) : mR(p[0]), mG(p[1]), mB(p[2]), mA(p[3]) {}

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! X
    constexpr float&       r()       { return mR; }
    constexpr const float& r() const { return mR; }

    //! Y
    constexpr float&       g()       { return mG; }
    constexpr const float& g() const { return mG; }

    //! Z
    constexpr float&       b()       { return mB; }
    constexpr const float& b() const { return mB; }

    //! W
    constexpr float&       a()       { return mA; }
    constexpr const float& a() const { return mA; }

    //-------------------------------------------------------------------------
    // cast
    //-------------------------------------------------------------------------
public:
    //! 配列キャスト
    constexpr operator       float* ()       { return &mR; }
    constexpr operator const float* () const { return &mR; }

    //-------------------------------------------------------------------------
    // assignment operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Color& operator *= (float rhs);
    inline constexpr Color& operator /= (float rhs);

    //-------------------------------------------------------------------------
    // binary operators
    //-------------------------------------------------------------------------
public:
    inline constexpr Color operator * (float rhs) const;
    inline constexpr Color operator / (float rhs) const;

    //-------------------------------------------------------------------------
    // equals operators
    //-------------------------------------------------------------------------
public:
    inline constexpr bool operator == (const Color& rhs) const;
    inline constexpr bool operator != (const Color& rhs) const;

    //-------------------------------------------------------------------------
private:
    float mR = 0.f;
    float mG = 0.f;
    float mB = 0.f;
    float mA = 1.f;
};

//-----------------------------------------------------------------------------
// 定数
//-----------------------------------------------------------------------------
static constexpr Color cBlack(0.f, 0.f, 0.f);

} // bel::

//-----------------------------------------------------------------------------
// binary operators
//-----------------------------------------------------------------------------
static constexpr bel::Color operator * (float f, const bel::Color& rhs);

#include "math/belColor.inl.h"
