/*!
 * @file   belGraphics.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// C++
#include <array>
#define NOMINMAX
#include <Windows.h>
// bel
#include "gfx/core/belCommandList.h"
#include "gfx/core/belCommandQueue.h"
#include "gfx/core/belRenderTarget.h"
#include "gfx/core/belRootSignature.h"
#include "gfx/core/belTexture.h"
#include "gfx/core/belTextureDescriptorRegistry.h"
#include "gfx/belGraphics.h"
#include "platform/belPlatform.h"

namespace bel
{
//-----------------------------------------------------------------------------
Graphics::Graphics() {}
Graphics::~Graphics() {}
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

    // デバイス取得（DirectX 12.2 のみ）
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_12_2;
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
    mpCommandQueue = std::make_unique<gfx::CommandQueue>();
    if (!mpCommandQueue->initialize(D3D12_COMMAND_LIST_TYPE_DIRECT))
    {
        return false;
    }

    // コマンドバッファー
    // @note: ダブルバッファー用に2つ用意して、交互に実行する
    mpCommandLists = std::make_unique<gfx::CommandList[]>(mNumBuffer);
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        if (!mpCommandLists[i_buffer].initialize(D3D12_COMMAND_LIST_TYPE_DIRECT))
        {
            return false;
        }
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
            &mpCommandQueue->getCommandQueue(),
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

    // テクスチャーレジストリーを作る
    if (!gfx::TextureDescriptorRegistry::GetInstance().allocate(cMaxTextureHandle))
    {
        BEL_ERROR_WINDOW("GraphicsError", "テクスチャー用デスクリプターヒープの作成に失敗しました");
        return false;
    }

    // ルートシグネチャは使いまわすので、ここで作っておく
    if (!gfx::RootSignature::GetInstance().create())
    {
        BEL_ERROR_WINDOW("GraphicsError", "ルートシグネチャの作成に失敗しました");
        return false;
    }

    // スワップチェーンからテクスチャーを取得
    mpColorBuffers  = std::make_unique<gfx::Texture[]>(mNumBuffer);
    mpRenderTargets = std::make_unique<gfx::RenderTarget[]>(mNumBuffer);
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        // スワップチェーンのリソースからテクスチャーを作る
        Microsoft::WRL::ComPtr<ID3D12Resource> p_color_buffer;
        if (FAILED(mpSwapChain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_color_buffer))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "出力バッファーの取得に失敗しました");
            return false;
        }
        mpColorBuffers[i_buffer].initializeFromResource(
            Platform::GetInstance().getWindowWidth(),
            Platform::GetInstance().getWindowHeight(),
            1,
            1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            gfx::Texture::cDimension2D,
            std::move(p_color_buffer)
        );

        // このテクスチャーを使ってレンダーターゲットを作る
        mpRenderTargets[i_buffer].initialize(mpColorBuffers[i_buffer]);
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
}
//-----------------------------------------------------------------------------
void Graphics::executeCommand()
{
    // カレントのコマンドリスト
    gfx::CommandList& curr_command_list = mpCommandLists[mCurrentBufferIndex];

    // コマンド記録開始
    curr_command_list.begin();

    // 初めに共通のルートシグネチャを設定
    curr_command_list.getCommandList().SetGraphicsRootSignature(
        &gfx::RootSignature::GetInstance().getGraphicsRootSignature()
    );
    curr_command_list.getCommandList().SetComputeRootSignature(
        &gfx::RootSignature::GetInstance().getComputeRootSignature()
    );

    // リソースバリア（Present -> RenderTarget）
    curr_command_list.resourceBarrierTransition(
        &mpColorBuffers[mCurrentBufferIndex].getResource(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    // 仮: グレーにクリア
    curr_command_list.clearColor(
        mpRenderTargets[mCurrentBufferIndex],
        math::Color(0.5f, 0.5f, 0.5f)
    );

    // リソースバリア（RenderTarget -> Present）
    curr_command_list.resourceBarrierTransition(
        &mpColorBuffers[mCurrentBufferIndex].getResource(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );

    // コマンドの実行
    curr_command_list.end();
    mpCommandQueue->executeCommand(curr_command_list);

    // 実行完了チェックのフェンス追加
    mpFences[mCurrentBufferIndex]->Signal(0);
    mpCommandQueue->signal(mpFences[mCurrentBufferIndex].Get(), 1);

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
    // オブジェクトの破棄を開始する前に、コマンドの実行完了を待つ
    for (uint32_t i_buffer = 0; i_buffer < mNumBuffer; ++i_buffer)
    {
        mCurrentBufferIndex = i_buffer;
        waitToExecuteCommand();
    }

    // テクスチャー削除は先に行う
    mpRenderTargets.reset();
    mpColorBuffers.reset();
}
//-----------------------------------------------------------------------------
}
