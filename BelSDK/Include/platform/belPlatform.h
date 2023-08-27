/*!
 * @file   belPlatform.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <memory>
#include <string>
#include <thread>
#include <Windows.h>
// bel
#include "memory/belSingleton.h"

namespace bel
{

//-----------------------------------------------------------------------------
/*!
 * �v���b�g�t�H�[���̓����������s���܂�
 */
class Platform : public Singleton<Platform>
{
public:
    /*!
     * �E�B���h�E�𐶐�����
     * @param[in] title �E�B���h�E��
     */
    void createWindow(const std::string& title);

    /*!
     * �A�v���P�[�V�������[�v�ɓ���܂�
     * @note ���̊֐����ĂԂƁAsetExit() ���ĂԂ܂ŕԂ��Ă��܂���
     */
    void enterApplicationLoop();

    /*!
     * �I�����ׂ��Ȃ�Ăяo���܂�
     */
    void setExit(int exit_code) { mExit = true; mExitCode = exit_code; }

    /*!
     * �I�����ׂ����`�F�b�N
     */
    bool isExit() const { return mExit; }

    //-------------------------------------------------------------------------
    // getter
    //-------------------------------------------------------------------------
public:
    //! �E�B���h�E�n���h��
    const HWND& getWindowHandle() const { return mWindowHandle; }
    //! ����
    uint32_t getWindowWidth() const { return mWidth; }
    //! ����
    uint32_t getWindowHeight() const { return mHeight; }

    //-------------------------------------------------------------------------
private:
    HWND mWindowHandle = HWND();
    std::unique_ptr<std::thread> mpWindowMsgThread;

    uint32_t mWidth  = 1920;
    uint32_t mHeight = 1080;

    bool mExit     = true;
    int  mExitCode = 0;

    //-------------------------------------------------------------------------
private:
    /*!
     * �A�v���P�[�V�������[�v
     */
    void applicationLoop_();

private:
    friend class Singleton<Platform>;
    Platform() {};
};
//-----------------------------------------------------------------------------

}
