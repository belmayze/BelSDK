/*!
 * @file   belApplicationWindowWin.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "base/belApplicationWindowWin.h"

namespace
{
//-----------------------------------------------------------------------------
//! ウィンドウイベント
LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY || msg == WM_CLOSE) { PostQuitMessage(0); return 0; }
    return DefWindowProc(handle, msg, wparam, lparam);
}
//-----------------------------------------------------------------------------
}

namespace bel {
//-----------------------------------------------------------------------------
bool ApplicationWindowWin::createWindow(const char* title, const char* class_name, uint32_t width, uint32_t height)
{
    // ウィンドウ名とクラス名の変換
    size_t title_strlen = (title      ? std::strlen(title)      : std::strlen("belmayze")) + 1; // len + null
    size_t class_strlen = (class_name ? std::strlen(class_name) : std::strlen("belmayze")) + 1;

    std::unique_ptr<TCHAR[]> title_str = std::make_unique<TCHAR[]>(title_strlen);
    std::unique_ptr<TCHAR[]> class_str = std::make_unique<TCHAR[]>(class_strlen);

    size_t ret = 0;
#   ifdef UNICODE
    mbstowcs_s(&ret, title_str.get(), title_strlen, title      ? title      : "belmayze", title_strlen);
    mbstowcs_s(&ret, class_str.get(), class_strlen, class_name ? class_name : "belmayze", class_strlen);
#   else
    strcpy_s(title_str.get(), title_strlen, title      ? title      : "belmayze");
    strcpy_s(class_str.get(), class_strlen, class_name ? class_name : "belmayze");
#   endif // UNICODE

    // ウィンドウの登録
    HINSTANCE instance_handle = GetModuleHandle(nullptr);

    WNDCLASSEX wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = instance_handle;
    wcex.hIcon         = nullptr;
    wcex.hIconSm       = nullptr;
    wcex.hCursor       = nullptr;
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszMenuName  = nullptr;
    wcex.lpszClassName = class_str.get();
    if (!RegisterClassEx(&wcex))
    {
        return false;
    }

    // ウィンドウスタイル
    DWORD wnd_style    = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    DWORD wnd_style_ex = 0;

    // ウィンドウサイズを計算
    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = width;
    rect.bottom = height;
    AdjustWindowRectEx(&rect, wnd_style, FALSE, wnd_style_ex);

    // ウィンドウを生成
    mpWindowHandle = CreateWindowEx(
        wnd_style_ex,
        _T("belmayze"),
        title_str.get(),
        wnd_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right  - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instance_handle,
        nullptr
    );
    if (!mpWindowHandle)
    {
        return false;
    }

    // ウィンドウサイズ保持
    mRenderWidth  = width;
    mRenderHeight = height;

    // ウィンドウの表示
    ShowWindow(mpWindowHandle, SW_SHOWNORMAL);
    UpdateWindow(mpWindowHandle);

    return true;
}
//-----------------------------------------------------------------------------
void ApplicationWindowWin::setContentRoot(const std::string& path)
{
    std::unique_ptr<TCHAR[]> dir_path = std::make_unique<TCHAR[]>(MAX_PATH);

    if (path.empty())
    {
        // 設定がない場合は実行パスを設定する
        GetModuleFileName(nullptr, dir_path.get(), MAX_PATH);

        // 実行ルートパス / Content
        TCHAR* p = _tcsrchr(dir_path.get(), _T('\\'));
        if (p)
        {
            std::memcpy(p, _T("\\Content\\\0"), sizeof(TCHAR) * 10);
        }
    }
    else
    {
        // 設定がある場合はそのパスを設定する
        size_t ret = 0;
#       ifdef UNICODE
        mbstowcs_s(&ret, dir_path.get(), path.size() + 1, path.c_str(), MAX_PATH);
#       else
        strcpy_s(dir_path.get(), path.size() + 1, path.c_str());
#       endif // UNICODE
    }

    SetCurrentDirectory(dir_path.get());
}
//-----------------------------------------------------------------------------
bool ApplicationWindowWin::peekWindowMessage()
{
    // メッセージチェック
    MSG msg;
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        DispatchMessage(&msg);
    }
    return msg.message == WM_QUIT;
}
//-----------------------------------------------------------------------------
void ApplicationWindowWin::destroyWindow()
{
    ShowWindow(mpWindowHandle, SW_HIDE);
    DestroyWindow(mpWindowHandle);
    mpWindowHandle = HWND();
}
//-----------------------------------------------------------------------------
}