/*!
 * @file   belPlatform.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <functional>
#include <locale>
#include <memory>
#include <tchar.h>
#define NOMINMAX
#include <Windows.h>
// bel
#include "gfx/belGraphics.h"
#include "platform/belPlatform.h"

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

namespace bel
{
//-----------------------------------------------------------------------------
void Platform::createWindow(const std::string& title)
{
    // ロケーションを日本語に
    setlocale(LC_ALL, "");

    // ウィンドウ名を生成
    size_t title_strlen = title.length() + 1; // len + null
    std::unique_ptr<TCHAR[]> windw_name = std::make_unique<TCHAR[]>(title_strlen);

    size_t ret = 0;
#   ifdef UNICODE
    mbstowcs_s(&ret, windw_name.get(), title_strlen, title.c_str(), title_strlen);
#   else
    strcpy_s(windw_name.get(), title_strlen, title.c_str());
#   endif // UNICODE

    // ウィンドウの登録
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
        BEL_ERROR_WINDOW("ERROR", "ウィンドウの登録に失敗しました");
        return;
    }

    // ウィンドウスタイル
    DWORD wnd_style    = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    DWORD wnd_style_ex = 0;

    // ウィンドウサイズを計算
    RECT rect;
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = mWidth;
    rect.bottom = mHeight;
    AdjustWindowRectEx(&rect, wnd_style, FALSE, wnd_style_ex);

    // ウィンドウを生成
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
        BEL_ERROR_WINDOW("ERROR", "ウィンドウの生成に失敗しました");
        return;
    }

    // グラフィックスの初期化
    if (!Graphics::GetInstance().initialize())
    {
        return;
    }

    // ウィンドウの表示
    ShowWindow(mWindowHandle, SW_SHOWNORMAL);
    UpdateWindow(mWindowHandle);

    mExit = false;
}
//-----------------------------------------------------------------------------
void Platform::enterApplicationLoop()
{
    // 終了フラグが立ってたら、ここで終わる
    if (mExit) { return; }

    // アプリケーションループ開始
    std::thread app_thread(std::bind(&Platform::applicationLoop_, this));

    // ウィンドウメッセージ確認
    MSG msg;
    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
        }
        // 占有しないように sleep を入れる
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } while (msg.message != WM_QUIT);

    // ループが抜けたらウィンドウが閉じられたので終了
    ShowWindow(mWindowHandle, SW_HIDE);
    mExit = true;

    // スレッドの終了待ち
    app_thread.join();
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
void Platform::applicationLoop_()
{
    while (!mExit)
    {
        // コマンドが実行されるまで待機
        Graphics::GetInstance().waitToExecuteCommand();

        // @TODO:

        // コマンド発行
        Graphics::GetInstance().executeCommand();

        // 画面の反映
        Graphics::GetInstance().present();
    }

    // 処理が終わったら終了処理
    Graphics::GetInstance().finalize();
}
//-----------------------------------------------------------------------------
}
