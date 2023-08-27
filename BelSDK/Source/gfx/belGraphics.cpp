/*!
 * @file   belGraphics.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <Windows.h>
// bel
#include "gfx/belGraphics.h"
#include "platform/belPlatform.h"

namespace bel
{
//-----------------------------------------------------------------------------
bool Graphics::initialize()
{
    // フラグ
    UINT dxgi_factory_flags = 0;

    // デバッグレイヤー
#   if BEL_IS_DEBUG()
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> p_debug_controller;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&p_debug_controller))))
        {
            p_debug_controller->EnableDebugLayer();
            dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#   endif // BEL_IS_DEBUG()

    // DXGI
    Microsoft::WRL::ComPtr<IDXGIFactory7> p_factory;
    if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&p_factory))))
    {
        BEL_ERROR_WINDOW("GraphicsError", "DXGIの生成に失敗しました");
        return false;
    }

    // デバイス取得（DirectX 12.1 のみ）
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_12_1;
    Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
    Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
    {
        bool found_adapter = false;
        for (uint32_t i_adapter = 0; !found_adapter && p_factory->EnumAdapters1(i_adapter, p_adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i_adapter)
        {
            DXGI_ADAPTER_DESC1 desc;
            p_adapter->GetDesc1(&desc);

            if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) != 0)
            {
                // ソフトウェアエミュレートは除外
                p_adapter.Reset();
                continue;
            }
            else
            {
                if (SUCCEEDED(D3D12CreateDevice(p_adapter.Get(), feature_level, IID_PPV_ARGS(&p_device))))
                {
                    found_adapter = true;
                }
            }
        }

        // デバイスが見つからなければ終了
        if (!found_adapter)
        {
            BEL_ERROR_WINDOW("GraphicsError", "DirectX のバージョンが不足しています");
            return false;
        }
    }
    mpDevice = std::move(p_device);

    // プライマリーコマンドを生成
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        if (FAILED(mpDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mpCommandQueue))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "コマンドキューの生成に失敗しました");
            return false;
        }
    }

    // スワップチェーン
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width              = Platform::GetInstance().getWindowWidth();
        desc.Height             = Platform::GetInstance().getWindowHeight();
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = 2;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.Flags              = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
        fullscreen_desc.RefreshRate.Numerator   = 60;
        fullscreen_desc.RefreshRate.Denominator = 1;
        fullscreen_desc.Windowed                = TRUE;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> p_tmp_swap_chain;
        if (FAILED(p_factory->CreateSwapChainForHwnd(
            mpCommandQueue.Get(),
            Platform::GetInstance().getWindowHandle(),
            &desc,
            &fullscreen_desc,
            nullptr,
            &p_tmp_swap_chain
        )))
        {
            BEL_ERROR_WINDOW("GraphicsError", "画面出力設定に失敗しました");
            return false;
        }

        if (FAILED(p_tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&mpSwapChain))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "画面出力のインターフェース取得に失敗しました");
            return false;
        }
        // sRGB
        mpSwapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709);
    }

    return true;
}
//-----------------------------------------------------------------------------
}
