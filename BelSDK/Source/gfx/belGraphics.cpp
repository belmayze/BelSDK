/*!
 * @file   belGraphics.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <array>
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

    // コマンドバッファー
    mpCommandAllocators = std::make_unique<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>[]>(mNumBuffer);
    mpCommandLists = std::make_unique<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>[]>(mNumBuffer);
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> p_command_allocator;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> p_command_list;
        if (FAILED(mpDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&p_command_allocator))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "コマンドアロケーターの生成に失敗しました");
            return false;
        }
        if (FAILED(mpDevice->CreateCommandList(1, D3D12_COMMAND_LIST_TYPE_DIRECT, p_command_allocator.Get(), nullptr, IID_PPV_ARGS(&p_command_list))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "コマンドリストの生成に失敗しました");
            return false;
        }
        p_command_list->Close();

        mpCommandAllocators[i_buffer] = std::move(p_command_allocator);
        mpCommandLists[i_buffer] = std::move(p_command_list);
    }

    // プライマリーコマンド実行のフェンスを作成
    mpFences = std::make_unique<Microsoft::WRL::ComPtr<ID3D12Fence>[]>(mNumBuffer);
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
        if (FAILED(mpDevice->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "フェンスの生成に失敗しました");
            return false;
        }
        mpFences[i_buffer] = std::move(p_fence);
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
        desc.BufferCount        = mNumBuffer;
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

    // スワップチェーンからテクスチャーを取得
    mpColorBuffers = std::make_unique<Microsoft::WRL::ComPtr<ID3D12Resource>[]>(mNumBuffer);
    mpRenderTargetDescriptorHeaps = std::make_unique<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>[]>(mNumBuffer);
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        Microsoft::WRL::ComPtr<ID3D12Resource> p_color_buffer;
        if (FAILED(mpSwapChain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_color_buffer))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "出力バッファーの取得に失敗しました");
            return false;
        }
        mpColorBuffers[i_buffer] = std::move(p_color_buffer);

        // このテクスチャーを使ってレンダーターゲットを作る
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
        {
            D3D12_DESCRIPTOR_HEAP_DESC desc = {};
            desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            desc.NumDescriptors = 1;
            if (FAILED(mpDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
            {
                BEL_ERROR_WINDOW("GraphicsError", "レンダーターゲットのデスクリプターヒープの生成に失敗しました");
                return false;
            }
        }
        {
            D3D12_RENDER_TARGET_VIEW_DESC desc = {};
            desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            mpDevice->CreateRenderTargetView(mpColorBuffers[i_buffer].Get(), &desc, p_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
        }
        mpRenderTargetDescriptorHeaps[i_buffer] = std::move(p_descriptor_heap);
    }

    return true;
}
//-----------------------------------------------------------------------------
void Graphics::waitToExecuteCommand()
{
    HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!handle)
    {
        BEL_ERROR_WINDOW("GraphicsError", "ハンドルの生成に失敗しました");
        return;
    }

    // 実行時に渡したフェンスが 1 になるまで待機する
    mpFences[mCurrentBufferIndex]->SetEventOnCompletion(1, handle);
    if (mpFences[mCurrentBufferIndex]->GetCompletedValue() < 1)
    {
        WaitForSingleObject(handle, INFINITE);
    }
    CloseHandle(handle);

    // コマンド記録開始
    mpCommandAllocators[mCurrentBufferIndex]->Reset();
    mpCommandLists[mCurrentBufferIndex]->Reset(mpCommandAllocators[mCurrentBufferIndex].Get(), nullptr);
}
//-----------------------------------------------------------------------------
void Graphics::executeCommand()
{
    // リソースバリア（Present -> RenderTarget）
    D3D12_RESOURCE_BARRIER desc = {};
    desc.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    desc.Transition.pResource   = mpColorBuffers[mCurrentBufferIndex].Get();
    desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
    mpCommandLists[mCurrentBufferIndex]->ResourceBarrier(1, &desc);

    // 仮: 青色にクリア
    float color[4] = {0.f, 0.125f, 0.5f, 1.f};
    mpCommandLists[mCurrentBufferIndex]->ClearRenderTargetView(
        mpRenderTargetDescriptorHeaps[mCurrentBufferIndex]->GetCPUDescriptorHandleForHeapStart(),
        color, 0U, nullptr
    );

    // リソースバリア（RenderTarget -> Present）
    desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    desc.Transition.StateAfter  = D3D12_RESOURCE_STATE_PRESENT;
    mpCommandLists[mCurrentBufferIndex]->ResourceBarrier(1, &desc);

    // コマンドの実行
    mpCommandLists[mCurrentBufferIndex]->Close();
    std::array<ID3D12CommandList*, 1> command_lists = {mpCommandLists[mCurrentBufferIndex].Get()};
    mpCommandQueue->ExecuteCommandLists(1, command_lists.data());

    // 実行完了チェックのフェンス追加
    mpFences[mCurrentBufferIndex]->Signal(0);
    mpCommandQueue->Signal(mpFences[mCurrentBufferIndex].Get(), 1);

    // バッファー切り替え
    mCurrentBufferIndex = 1 - mCurrentBufferIndex;
}
//-----------------------------------------------------------------------------
void Graphics::present()
{
    // 画面の反映（VSync 待ち）
    mpSwapChain->Present(1, 0);
}
//-----------------------------------------------------------------------------
void Graphics::finalize()
{
    // キューの実行待ち
    {
        // フェンスを作ってコマンドに入れる
        Microsoft::WRL::ComPtr<ID3D12Fence> p_fence;
        if (FAILED(mpDevice->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&p_fence))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "フェンスの生成に失敗しました")
            return;
        }
        p_fence->Signal(0);
        mpCommandQueue->Signal(p_fence.Get(), 1);

        // 実行待ち
        HANDLE handle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!handle)
        {
            BEL_ERROR_WINDOW("GraphicsError", "ハンドルの生成に失敗しました");
            return;
        }
        p_fence->SetEventOnCompletion(1, handle);
        WaitForSingleObject(handle, INFINITE);
        CloseHandle(handle);
    }

    // バッファー破棄
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        mpColorBuffers[i_buffer].Reset();
        mpFences[i_buffer].Reset();
        mpCommandLists[i_buffer].Reset();
        mpCommandAllocators[i_buffer].Reset();
    }

    // 破棄
    mpCommandQueue.Reset();
    mpSwapChain.Reset();
    mpDevice.Reset();
}
//-----------------------------------------------------------------------------
}
