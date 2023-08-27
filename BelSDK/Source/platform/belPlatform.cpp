/*!
 * @file   belPlatform.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <functional>
#include <memory>
#include <tchar.h>
#include <Windows.h>
// bel
#include "platform/belPlatform.h"

namespace
{
//-----------------------------------------------------------------------------
//! �E�B���h�E�C�x���g
LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY || msg == WM_CLOSE) { PostQuitMessage(0); return 0; }
    return DefWindowProc(handle, msg, wparam, lparam);
}
//-----------------------------------------------------------------------------
}

namespace bel
{
//-----------------------------------------------------------------------------
void Platform::createWindow(const std::string& title)
{
    // �E�B���h�E���𐶐�
    size_t title_strlen = title.length() + 1; // len + null
    std::unique_ptr<TCHAR[]> windw_name = std::make_unique<TCHAR[]>(title_strlen);

    size_t ret = 0;
#   ifdef UNICODE
    mbstowcs_s(&ret, windw_name.get(), title_strlen, title.c_str(), title_strlen);
#   else
    strcpy_s(windw_name.get(), title_strlen, title.c_str());
#   endif // UNICODE

    // �E�B���h�E�̓o�^
    HINSTANCE handle = GetModuleHandle(nullptr);

    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = handle;
    wcex.hIcon         = nullptr;
    wcex.hIconSm       = nullptr;
    wcex.hCursor       = nullptr;
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = _T("belmayze");
    if (!RegisterClassEx(&wcex))
    {
        BEL_ERROR_WINDOW("ERROR", "�E�B���h�E�̓o�^�Ɏ��s���܂���");
        return;
    }

    // �E�B���h�E�X�^�C��
    DWORD wnd_style    = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    DWORD wnd_style_ex = 0;

    // �E�B���h�E�T�C�Y���v�Z
    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = mWidth;
    rect.bottom = mHeight;
    AdjustWindowRectEx(&rect, wnd_style, FALSE, wnd_style_ex);

    // �E�B���h�E�𐶐�
    mWindowHandle = CreateWindowEx(
        wnd_style_ex,
        _T("belmayze"),
        windw_name.get(),
        wnd_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right  - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        handle,
        nullptr
    );
    if (!mWindowHandle)
    {
        BEL_ERROR_WINDOW("ERROR", "�E�B���h�E�̐����Ɏ��s���܂���");
        return;
    }

    // �E�B���h�E�̕\��
    ShowWindow(mWindowHandle, SW_SHOWNORMAL);
    UpdateWindow(mWindowHandle);
}
//-----------------------------------------------------------------------------
void Platform::enterApplicationLoop()
{
    // �A�v���P�[�V�������[�v�J�n
    std::thread app_thread(std::bind(&Platform::applicationLoop_, this));

    // �E�B���h�E���b�Z�[�W�m�F
    MSG msg;
    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (msg.message != WM_QUIT);

    // ���[�v����������E�B���h�E������ꂽ�̂ŏI��
    ShowWindow(mWindowHandle, SW_HIDE);
    mExit = true;

    // �X���b�h�̏I���҂�
    app_thread.join();
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
void Platform::applicationLoop_()
{
    while (!mExit)
    {
        // @TODO:

    }
}
//-----------------------------------------------------------------------------
}
