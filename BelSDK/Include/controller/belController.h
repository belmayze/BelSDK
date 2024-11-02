/*!
 * @file   belController.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// bel
#include "controller/belControllerButtonType.h"

namespace bel::con {
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
};
//-----------------------------------------------------------------------------
} // bel::con::
