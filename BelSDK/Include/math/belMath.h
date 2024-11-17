/*!
 * @file   belMath.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel {
//-----------------------------------------------------------------------------
class Math
{
    //-------------------------------------------------------------------------
    // 定数
    //-------------------------------------------------------------------------
public:
    //! 円周率
    static constexpr float cPi   = 3.1415927f;
    //! 円周率 * 2
    static constexpr float c2Pi  = 6.2831853f;
    //! 円周率 / 2
    static constexpr float cPi_2 = 1.5707963f;

    //-------------------------------------------------------------------------
    // 関数
    //-------------------------------------------------------------------------
public:
    /*!
     * 切り上げ
     * @note C++23 から constexpr にできる
     * @param[in] a     入力
     * @param[in] times 倍数
     */
    static inline int Roundup(float a, float times)
    {
        float t = a / times;
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline int Roundup(int a, int times)
    {
        float t = static_cast<float>(a) / static_cast<float>(times);
        return static_cast<int>(std::ceil(t) * times);
    }
    static inline size_t Roundup(size_t a, size_t times)
    {
        double t = static_cast<double>(a) / static_cast<double>(times);
        return static_cast<size_t>(std::ceil(t) * times);
    }

    /*!
     * 最大、最小
     */
    template <typename T>
    static constexpr T Min(T x, T y) { return x < y ? x : y; }

    template <typename T>
    static constexpr T Max(T x, T y) { return x > y ? x : y; }

    template <typename T>
    static constexpr T Clamp(T x, T min, T max) { return x < min ? min : (x > max ? max : x); }

    //-------------------------------------------------------------------------
    // カラー
    //-------------------------------------------------------------------------
public:
    //! 輝度計算
    //! @note 与える色はリニアガンマです
    static constexpr float CalcLuminance(float r, float g, float b)
    {
        return r * 0.2126729f + g * 0.7151522f + b * 0.0721750f;
    }
    //! sRGB -> Linear
    //! constexpr は C++26 から
    static inline float ConvertSRGBToLinear(float v)
    {
        if (v <= 0.04045f) { return v / 12.92f; }
        else               { return std::pow((v + 0.055f) / 1.055f, 2.4f); }
    }
    //! Linear -> sRGB
    //! constexpr は C++26 から
    static inline float ConvertLinearToSRGB(float v)
    {
        if (v <= 0.0031308f) { return v * 12.92f; }
        else                 { return 1.055f * std::pow(v, 1.f / 2.4f) - 0.055f; }
    }

    //-------------------------------------------------------------------------
    // 補間
    //-------------------------------------------------------------------------
public:
    //! 線形補間
    static constexpr float Mix(float a, float b, float t)
    {
        return std::lerp(a, b, t);
    }

    //! easeIn
    //! constexpr は C++26 から
    static inline float EaseInSine(float x)
    {
        return 1.f - std::cos((x * cPi) * 0.5f);
    }
    static constexpr float EaseInQuad(float x)
    {
        return x * x;
    }
    static constexpr float EaseInCubic(float x)
    {
        return x * x * x;
    }
    static constexpr float EaseInQuart(float x)
    {
        float x2 = x * x;
        return x2 * x2;
    }

    //! easeOut
    //! constexpr は C++26 から
    static inline float EaseOutSine(float x)
    {
        return std::sin((x * cPi) * 0.5f);
    }
    static constexpr float EaseOutQuad(float x)
    {
        float one_minus = 1.f - x;
        return 1.f - one_minus * one_minus;
    }
    static constexpr float EaseOutCubic(float x)
    {
        float one_minus = 1.f - x;
        return 1.f - one_minus * one_minus * one_minus;
    }
    static constexpr float EaseOutQuart(float x)
    {
        float one_minus  = 1.f - x;
        float one_minus2 = one_minus * one_minus;
        return 1.f - one_minus2 * one_minus2;
    }

    //! easeInOut
    //! constexpr は C++26 から
    static inline float EaseInOutSine(float x)
    {
        return -(std::cos(x * cPi) - 1.f) * 0.5f;
    }
    static constexpr float EaseInOutQuad(float x)
    {
        if (x < 0.5f)
        {
            return 2.f * x * x;
        }
        else
        {
            float f = -2.f * x + 2.f;
            return 1.f - f * f * 0.5f;
        }
    }
    static constexpr float EaseInOutCubic(float x)
    {
        if (x < 0.5f)
        {
            return 4.f * x * x * x;
        }
        else
        {
            float f = -2.f * x + 2.f;
            return 1.f - f * f * f * 0.5f;
        }
    }
    static constexpr float EaseInOutQuart(float x)
    {
        if (x < 0.5f)
        {
            float x2 = x * x;
            return 8.f * x2 * x2;
        }
        else
        {
            float f = -2.f * x + 2.f;
            float f2 = f * f;
            return 1.f - f2 * f2 * 0.5f;
        }
    }

};
//-----------------------------------------------------------------------------
}

// math includes
#include "math/belDegree.h"
#include "math/belRadian.h"

#include "math/belColor.h"
#include "math/belVector2.h"
#include "math/belVector3.h"
#include "math/belVector4.h"
#include "math/belMatrix34.h"
#include "math/belMatrix44.h"
