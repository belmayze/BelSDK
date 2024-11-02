/*!
 * @file   belController.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "hid/belController.h"

namespace bel::hid {
//-----------------------------------------------------------------------------
void Controller::update(uint32_t index)
{
    // ステートリセット
    EnumFlags<ButtonType> prev_flags = mPressFlags;
    mPressFlags.reset();

    // コントローラーの状態取得
    XINPUT_STATE state{};
    DWORD result = XInputGetState(index, &state);
    if (result == ERROR_SUCCESS)
    {
        // 接続されている
        mStateFlags.set(State::cConnected);

        // ボタン
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A)              != 0) { mPressFlags.set(ButtonType::cA,         true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B)              != 0) { mPressFlags.set(ButtonType::cB,         true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X)              != 0) { mPressFlags.set(ButtonType::cX,         true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)              != 0) { mPressFlags.set(ButtonType::cY,         true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)        != 0) { mPressFlags.set(ButtonType::cDPadUp,    true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)      != 0) { mPressFlags.set(ButtonType::cDPadDown,  true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)      != 0) { mPressFlags.set(ButtonType::cDPadLeft,  true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)     != 0) { mPressFlags.set(ButtonType::cDPadRight, true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_START)          != 0) { mPressFlags.set(ButtonType::cStart,     true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)           != 0) { mPressFlags.set(ButtonType::cBack,      true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)  != 0) { mPressFlags.set(ButtonType::cL1,        true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) { mPressFlags.set(ButtonType::cR1,        true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)     != 0) { mPressFlags.set(ButtonType::cL3,        true); }
        if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)    != 0) { mPressFlags.set(ButtonType::cR3,        true); }

        // トリガー
        mLeftTrigger  = static_cast<float>(state.Gamepad.bLeftTrigger)  / 255.f;
        mRightTrigger = static_cast<float>(state.Gamepad.bRightTrigger) / 255.f;
        // トリガーは一定量以上になったらボタンとして機能する
        if (state.Gamepad.bLeftTrigger  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) { mPressFlags.set(ButtonType::cL2, true); }
        if (state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) { mPressFlags.set(ButtonType::cR2, true); }

        // ボタンのステート変更
        {
            EnumFlags<ButtonType> change_flags = prev_flags ^ mPressFlags;
            mTriggerFlags = change_flags & mPressFlags;
            mReleaseFlags = change_flags & prev_flags;
        }

        // スティック
        {
            constexpr float cLeftStickRangeInv  = 1.f / static_cast<float>(std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
            constexpr float cRightStickRangeInv = 1.f / static_cast<float>(std::numeric_limits<int16_t>::max() - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

            // 左スティック X
            if (state.Gamepad.sThumbLX >= 0)
            {
                int16_t value = Math::Max<int16_t>(state.Gamepad.sThumbLX - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
                mLeftStick.x() = value * cLeftStickRangeInv;
            }
            else
            {
                int16_t value = Math::Min<int16_t>(state.Gamepad.sThumbLX + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
                mLeftStick.x() = value * cLeftStickRangeInv;
            }
            // 左スティック Y
            if (state.Gamepad.sThumbLY >= 0)
            {
                int16_t value = Math::Max<int16_t>(state.Gamepad.sThumbLY - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
                mLeftStick.y() = value * cLeftStickRangeInv;
            }
            else
            {
                int16_t value = Math::Min<int16_t>(state.Gamepad.sThumbLY + XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, 0);
                mLeftStick.y() = value * cLeftStickRangeInv;
            }

            // 右スティック X
            if (state.Gamepad.sThumbRX >= 0)
            {
                int16_t value = Math::Max<int16_t>(state.Gamepad.sThumbRX - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
                mRightStick.x() = value * cRightStickRangeInv;
            }
            else
            {
                int16_t value = Math::Min<int16_t>(state.Gamepad.sThumbRX + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
                mRightStick.x() = value * cRightStickRangeInv;
            }
            // 右スティック Y
            if (state.Gamepad.sThumbRY >= 0)
            {
                int16_t value = Math::Max<int16_t>(state.Gamepad.sThumbRY - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
                mRightStick.y() = value * cRightStickRangeInv;
            }
            else
            {
                int16_t value = Math::Min<int16_t>(state.Gamepad.sThumbRY + XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE, 0);
                mRightStick.y() = value * cRightStickRangeInv;
            }
        }
    }
    else
    {
        // 接続状態をリセット
        mStateFlags.reset(State::cConnected);

        // アナログ入力の値をリセット
        mLeftStick.x()  = 0.f; mLeftStick.y()  = 0.f;
        mRightStick.x() = 0.f; mRightStick.y() = 0.f;
        mLeftTrigger    = 0.f; mRightTrigger   = 0.f;
    }

    // ボタンのステート変更
    {
        EnumFlags<ButtonType> change_flags = prev_flags ^ mPressFlags;
        mTriggerFlags = change_flags & mPressFlags;
        mReleaseFlags = change_flags & prev_flags;

        // @note Release のフラグが立っていたら Disconnected としない
        //       ボタンを押したまま接続を切ると Release にならないため
        if (!mStateFlags.test(State::cConnected) && mReleaseFlags.any())
        {
            mStateFlags.set(State::cConnected);
        }
    }
}
//-----------------------------------------------------------------------------
} // bel::hid::
