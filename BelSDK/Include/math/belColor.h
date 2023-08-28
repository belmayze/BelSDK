/*!
 * @file   belVector2.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once

namespace bel::math
{

class Color
{
public:
    //! コンストラクター
    Color() {}
    //! 任意の値で初期化します
    Color(float r, float g, float b, float a = 1.f) : mR(r), mG(g), mB(b), mA(a) {}

    //-------------------------------------------------------------------------
    // getter / setter
    //-------------------------------------------------------------------------
public:
    float& r() { return mR; }
    float& g() { return mG; }
    float& b() { return mB; }
    float& a() { return mA; }

    const float& r() const { return mR; }
    const float& g() const { return mG; }
    const float& b() const { return mB; }
    const float& a() const { return mA; }
    
private:
    float mR = 0.f;
    float mG = 0.f;
    float mB = 0.f;
    float mA = 1.f;
};

}
