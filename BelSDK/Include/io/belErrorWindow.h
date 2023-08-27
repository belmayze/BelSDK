/*!
 * @file   belErrorWindow.h
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */
#pragma once
// C++
#include <string>
// bel
#include "memory/belSingleton.h"

namespace bel::io
{
//-----------------------------------------------------------------------------
/*!
 * �G���[�E�B���h�E��\�����܂�
 */
class ErrorWindow : public Singleton<ErrorWindow>
{
public:
    /*!
     * �\��
     * @param[in] title   �^�C�g��
     * @param[in] message ���b�Z�[�W
     */
    void show(const std::string& title, const std::string& message);
};
//-----------------------------------------------------------------------------
}
