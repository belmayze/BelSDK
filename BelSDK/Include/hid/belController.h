/*!
 * @file   belController.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "hid/belControllerButtonType.h"

namespace bel::hid {
//-----------------------------------------------------------------------------
class Controller
{
    //-------------------------------------------------------------------------
    // ctor / dtor
    //-------------------------------------------------------------------------
public:
    //! 
    Controller() {}

    //-------------------------------------------------------------------------
    // update
    //-------------------------------------------------------------------------
public:
    //! 更新
    void update(uint32_t index);

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! 接続されているか
    bool isConnected() const { return mStateFlags.test(State::cConnected); }

    //! 押しているかどうか
    bool isPress(ButtonType type) const { return mPressFlags.test(type); }
    //! 押した瞬間かどうか
    bool isTrigger(ButtonType type) const { return mTriggerFlags.test(type); }
    //! 離した瞬間かどうか
    bool isRelease(ButtonType type) const { return mReleaseFlags.test(type); }

    //! 左スティック
    const Vector2& getLeftStick() const { return mLeftStick; }
    //! 右スティック
    const Vector2& getRightStick() const { return mRightStick; }
    //! 左トリガー
    float getLeftTrigger() const { return mLeftTrigger; }
    //! 右トリガー
    float getRightTrigger() const { return mRightTrigger; }

    //-------------------------------------------------------------------------
private:
    //! 状態
    enum class State : uint8_t
    {
        cConnected,
        cNum
    };

    //-------------------------------------------------------------------------
private:
    EnumFlags<State>      mStateFlags;   //!< 状態
    EnumFlags<ButtonType> mPressFlags;   //!< 押しているときにずっと立っているビット
    EnumFlags<ButtonType> mTriggerFlags; //!< 押した瞬間に立つビット
    EnumFlags<ButtonType> mReleaseFlags; //!< 離した瞬間に立つビット

    float   mLeftTrigger  = 0.f;
    float   mRightTrigger = 0.f;
    Vector2 mLeftStick;
    Vector2 mRightStick;
};
//-----------------------------------------------------------------------------
} // bel::hid::
