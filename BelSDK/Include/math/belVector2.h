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

class Vector2
{
public:
    //! コンストラクター
    Vector2() {}
    //! すべて同じ値で初期化します
    Vector2(float v) : mX(v), mY(v) {}
    //! 任意の値で初期化します
    Vector2(float x, float y) : mX(x), mY(y) {}

    //-------------------------------------------------------------------------
    // getter / setter
    //-------------------------------------------------------------------------
public:
    float& x() { return mX; }
    float& y() { return mY; }

    const float& x() const { return mX; }
    const float& y() const { return mY; }
    
private:
    float mX = 0.f;
    float mY = 0.f;
};

}
