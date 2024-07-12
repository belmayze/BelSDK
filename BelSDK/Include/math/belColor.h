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
    // 定数 (Rec.709 Color space / Linear gamma)
    //-------------------------------------------------------------------------
public:
    //! 黒
    static constexpr Color cBlack()       { return Color(0.000f, 0.000f, 0.000f); }
    //! 18% グレー
    static constexpr Color cGray()        { return Color(0.180f, 0.180f, 0.180f); }
    //! 白
    static constexpr Color cWhite()       { return Color(1.000f, 1.000f, 1.000f); }
    //! 赤
    static constexpr Color cRed()         { return Color(1.000f, 0.000f, 0.000f); }
    //! 緑
    static constexpr Color cGreen()       { return Color(0.000f, 0.214f, 0.000f); }
    //! 青
    static constexpr Color cBlue()        { return Color(0.000f, 0.000f, 1.000f); }
    //! 黄色
    static constexpr Color cYellow()      { return Color(1.000f, 1.000f, 0.000f); }
    //! シアン
    static constexpr Color cCyan()        { return Color(0.000f, 1.000f, 1.000f); }
    //! マゼンタ
    static constexpr Color cMagenta()     { return Color(1.000f, 0.000f, 1.000f); }
    //! オレンジ
    static constexpr Color cOrange()      { return Color(1.000f, 0.376f, 0.000f); }
    //! 黄緑
    static constexpr Color cYellowGreen() { return Color(0.323f, 0.610f, 0.032f); }
    //! ライム
    static constexpr Color cLime()        { return Color(0.000f, 1.000f, 0.000f); }
    //! 空色
    static constexpr Color cSkyBlue()     { return Color(0.242f, 0.617f, 0.831f); }
    //! 薄紫
    static constexpr Color cViolet()      { return Color(0.855f, 0.223f, 0.855f); }
    //! 紫
    static constexpr Color cPurple()      { return Color(0.214f, 0.000f, 0.214f); }

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
    // convert
    //-------------------------------------------------------------------------
public:
    inline Color convertToSRGBGamut() const; // constexpr は C++26 から
    inline Color convertToLinearGamut() const; // constexpr は C++26 から

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
