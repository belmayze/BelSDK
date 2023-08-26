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
    //! �R���X�g���N�^�[
    Vector2() {}
    //! ���ׂē����l�ŏ��������܂�
    Vector2(float v) : mX(v), mY(v) {}
    //! �C�ӂ̒l�ŏ��������܂�
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
