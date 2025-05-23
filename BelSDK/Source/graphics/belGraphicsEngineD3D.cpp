﻿/*!
 * @file   belGraphicsEngineD3D.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "base/belApplicationWindow.h"
#include "graphics/common/belGraphicsCommandContext.h"
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/common/belGraphicsCommandQueue.h"
#include "graphics/common/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/common/belGraphicsDynamicTextureResource.h"
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/dev/belGraphicsDevMeshHolder.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
#include "graphics/internal/belGraphicsTextureCopyQueue.h"
#include "graphics/belGraphicsEngineD3D.h"

namespace bel {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
GraphicsEngineD3D::GraphicsEngineD3D()
{
    // グラフィックスのシングルトンはここで生成する
    gfx::GlobalDescriptorRegistry::GetInstance();
    gfx::DynamicDescriptorHeap::GetInstance();
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool GraphicsEngineD3D::initialize()
{
    // フラグ
    UINT dxgi_factory_flags = 0;

    // デバッグレイヤー
#   if BEL_TARGET_IS_DEBUG()
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> p_debug;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&p_debug))))
        {
            p_debug->EnableDebugLayer();
            dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#   endif // BEL_TARGET_IS_DEBUG()

    // DXGI
    Microsoft::WRL::ComPtr<IDXGIFactory7> p_factory;
    if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&p_factory))))
    {
        BEL_ERROR_LOG("DXGIの生成に失敗しました\n");
        return false;
    }

    // デバイス取得
    {
        D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_12_1;
        Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
        Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
        Microsoft::WRL::ComPtr<IDXGIOutput>   p_tmp_output;
        Microsoft::WRL::ComPtr<IDXGIOutput6>  p_output;
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

            // 出力を取得
            if (FAILED(p_adapter->EnumOutputs(0, p_tmp_output.GetAddressOf())))
            {
                // 出力先が見つからない
                continue;
            }
            if (FAILED(p_tmp_output->QueryInterface(IID_PPV_ARGS(&p_output))))
            {
                continue;
            }

            if (SUCCEEDED(D3D12CreateDevice(p_adapter.Get(), feature_level, IID_PPV_ARGS(&p_device))))
            {
                found_adapter = true;

                // ディスプレイの情報を取得
                DXGI_OUTPUT_DESC1 output_desc;
                p_output->GetDesc1(&output_desc);

                mIsSupportedHDR = (output_desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);

                // HDR の場合は paper white を取得する
                if (mIsSupportedHDR)
                {
                    auto get_path_func = [&output_desc](DISPLAYCONFIG_PATH_INFO& info)
                        {
                            // 情報の最大数を取得
                            UINT32 num_path = 0;
                            UINT32 num_mode = 0;
                            if (FAILED(GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &num_path, &num_mode))) { return false; }

                            // 情報取得
                            auto path_infos = std::make_unique<DISPLAYCONFIG_PATH_INFO[]>(num_path);
                            auto mode_infos = std::make_unique<DISPLAYCONFIG_MODE_INFO[]>(num_mode);
                            if (FAILED(QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &num_path, path_infos.get(), &num_mode, mode_infos.get(), nullptr))) { return false; }

                            // 出力先と同じパス情報を列挙
                            for (UINT32 i_path = 0; i_path < num_path; ++i_path)
                            {
                                DISPLAYCONFIG_SOURCE_DEVICE_NAME dn = {};
                                dn.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
                                dn.header.adapterId = path_infos[i_path].sourceInfo.adapterId;
                                dn.header.id = path_infos[i_path].sourceInfo.id;
                                dn.header.size = sizeof(dn);
                                DisplayConfigGetDeviceInfo(&dn.header);

                                // デバイス名が一致したら情報をコピーして終了
                                if (wcscmp(dn.viewGdiDeviceName, output_desc.DeviceName) == 0)
                                {
                                    info = path_infos[i_path];
                                    return true;
                                }
                            }
                            return false;
                        };

                    if (get_path_func(mHDRPathInfo))
                    {
                        mHDRPathInfoEnabled = true;
                        updateHDRPaperWhite_();
                    }
                }
            }
        }

        if (!found_adapter)
        {
            BEL_ERROR_LOG("DirectX のバージョンが不足しています\n");
            return false;
        }
        mpDevice = std::move(p_device);
    }

    // デバッグレイヤーフィルター
    // 今は使用していない
#   if BEL_TARGET_IS_DEBUG() && 0
    {
        // デバッグレイヤーの特定の警告は無効にする
        Microsoft::WRL::ComPtr<ID3D12InfoQueue> p_info_queue;
        if (SUCCEEDED(mpDevice.As(&p_info_queue)))
        {
            // Infoはデフォルトで表示されないので登録
            D3D12_MESSAGE_SEVERITY severities[] =
            {
                D3D12_MESSAGE_SEVERITY_INFO
            };

            // 無効にするメッセージID
            D3D12_MESSAGE_ID deny_messages[] =
            {
                // 登録されている clear value と異なるため、クリア処理が遅くなるという警告
                // 使わないといけないケースもあるので無効にしている
                D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
            };

            // フィルターを作って登録
            D3D12_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumSeverities = std::size(severities);
            filter.DenyList.pSeverityList = severities;
            filter.DenyList.NumIDs = std::size(deny_messages);
            filter.DenyList.pIDList = deny_messages;
            if (FAILED(p_info_queue->PushStorageFilter(&filter)))
            {
                // このエラーは終了せず継続
                BEL_ERROR_LOG("デバッグレイヤーのフィルター登録に失敗しました");
            }
        }
    }
#   endif // BEL_TARGET_IS_DEBUG()

    // 機能チェック
    {
        if (FAILED(mpDevice->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &mFeatureOptions, sizeof(mFeatureOptions))))
        {
            BEL_ERROR_LOG("機能のサポートチェックに失敗しました\n");
            return false;
        }

        // 複数のヒープを持つ必要がある
        if (mFeatureOptions.ResourceHeapTier < D3D12_RESOURCE_HEAP_TIER_2)
        {
            BEL_ERROR_LOG("D3D12_RESOURCE_HEAP_TIER_2 のサポートが必要です\n");
            return false;
        }
    }

    // メインのコマンドキューとコマンドリストを作る
    {
        mpMainCommandQueue = std::make_unique<gfx::CommandQueue>();
        if (!mpMainCommandQueue->initialize(gfx::CommandType::cGraphics))
        {
            BEL_ERROR_LOG("メインコマンドキューの生成に失敗しました\n");
            return false;
        }
    }
    mpMainCommandLists  = std::make_unique<gfx::CommandList[]>(2);
    for (uint32_t i_buffer = 0; i_buffer < 2; ++i_buffer)
    {
        if (!mpMainCommandLists[i_buffer].initialize(gfx::CommandType::cGraphics))
        {
            BEL_ERROR_LOG("メインコマンドリストの生成に失敗しました\n");
            return false;
        }
    }

    // メインのコマンドキューを待機するフェンス
    {
        // フェンスを待機するハンドル
        mWaitFenceHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!mWaitFenceHandle)
        {
            BEL_ERROR_LOG("フェンス用イベントの生成に失敗しました\n");
            return false;
        }

        // メインのキュー実行完了チェック用フェンス
        if (FAILED(mpDevice->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence))))
        {
            BEL_ERROR_LOG("メインキュー用のフェンス生成に失敗しました\n");
            return false;
        }
    }

    // グローバルなデスクリプターを作る
    if (!gfx::GlobalDescriptorRegistry::GetInstance().initialize(cMaxTextureHandle))
    {
        BEL_ERROR_LOG("グローバルデスクリプターヒープの作成に失敗しました\n");
        return false;
    }

    // 動的確保を行うデスクリプターを作る
    if (!gfx::DynamicDescriptorHeap::GetInstance().initialize(cMaxDynamicDescriptorHandle))
    {
        BEL_ERROR_LOG("ダイナミックデスクリプターヒープの作成に失敗しました\n");
        return false;
    }

    // 動的確保を行うテクスチャーリソースを作る
    if (!gfx::DynamicTextureResource::GetInstance().initialize(cMaxDynamicTextureResourceSize))
    {
        BEL_ERROR_LOG("ダイナミックテクスチャーリソースの作成に失敗しました\n");
        return false;
    }

    // スワップチェーン
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width              = ApplicationWindow::GetInstance().getRenderWidth();
        desc.Height             = ApplicationWindow::GetInstance().getRenderHeight();
        desc.Format             = gfx::to_native(mIsSupportedHDR ? cHDRTextureFormat : cSDRTextureFormat);
        desc.Stereo             = FALSE;
        desc.SampleDesc.Count   = 1;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount        = cNumBuffer;
        desc.SwapEffect         = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreen_desc = {};
        fullscreen_desc.RefreshRate.Numerator   = 60;
        fullscreen_desc.RefreshRate.Denominator = 1;
        fullscreen_desc.Windowed                = TRUE;

        Microsoft::WRL::ComPtr<IDXGISwapChain1> p_tmp_swap_chain;
        if (FAILED(p_factory->CreateSwapChainForHwnd(
            &mpMainCommandQueue->getCommandQueue(),
            ApplicationWindow::GetInstance().getWindowHandle(),
            &desc, &fullscreen_desc,
            nullptr, &p_tmp_swap_chain
        )))
        {
            BEL_ERROR_LOG("画面出力設定に失敗しました\n");
            return false;
        }

        if (FAILED(p_tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&mpSwapChain))))
        {
            BEL_ERROR_LOG("画面出力のインターフェース取得に失敗しました\n");
            return false;
        }

        // カラースペースの設定
        mpSwapChain->SetColorSpace1(mIsSupportedHDR ? DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020
                                                    : DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709);

        // HDR の設定
        if (mIsSupportedHDR)
        {
            // 色空間情報
            constexpr DisplayChromaticity cDisplayChromaticities[] =
            {
                { 0.6400f, 0.3300f, 0.3000f, 0.6000f, 0.1500f, 0.0600f, 0.3127f, 0.3290f }, // Rec.709
                { 0.7080f, 0.2920f, 0.1700f, 0.7970f, 0.1310f, 0.0460f, 0.3127f, 0.3290f }  // Rec.2020
            };
            // 輝度情報 (DCI-P3 基準)
            constexpr float cMaxOutputNits = 1000.f;
            constexpr float cMinOutputNits = 0.001f;
            constexpr float cMaxCLL        = 2000.f;
            constexpr float cMaxFALL       = 500.f;

            DXGI_HDR_METADATA_HDR10 metadata = {};
            // D3D12HDR のサンプルコードから
            // https://msdn.microsoft.com/en-us/library/windows/desktop/mt732700(v=vs.85).aspx
            metadata.RedPrimary[0]   = static_cast<UINT16>(cDisplayChromaticities[1].red_x   * 50000.f);
            metadata.RedPrimary[1]   = static_cast<UINT16>(cDisplayChromaticities[1].red_y   * 50000.f);
            metadata.GreenPrimary[0] = static_cast<UINT16>(cDisplayChromaticities[1].green_x * 50000.f);
            metadata.GreenPrimary[1] = static_cast<UINT16>(cDisplayChromaticities[1].green_y * 50000.f);
            metadata.BluePrimary[0]  = static_cast<UINT16>(cDisplayChromaticities[1].blue_x  * 50000.f);
            metadata.BluePrimary[1]  = static_cast<UINT16>(cDisplayChromaticities[1].blue_y  * 50000.f);
            metadata.WhitePoint[0]   = static_cast<UINT16>(cDisplayChromaticities[1].white_x * 50000.f);
            metadata.WhitePoint[1]   = static_cast<UINT16>(cDisplayChromaticities[1].white_y * 50000.f);
            metadata.MaxMasteringLuminance     = static_cast<UINT>(cMaxOutputNits * 10000.f);
            metadata.MinMasteringLuminance     = static_cast<UINT>(cMinOutputNits * 10000.f);
            metadata.MaxContentLightLevel      = static_cast<UINT16>(cMaxCLL);
            metadata.MaxFrameAverageLightLevel = static_cast<UINT16>(cMaxFALL);
            mpSwapChain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10, sizeof(metadata), &metadata);
        }

        // スワップチェーンからテクスチャーを取得する
        mSwapChainTextures = std::make_unique<gfx::Texture[]>(cNumBuffer);
        mSwapChainRenderTargets = std::make_unique<gfx::RenderTarget[]>(cNumBuffer);
        mSwapChainRenderBuffers = std::make_unique<gfx::RenderBuffer[]>(cNumBuffer);
        for (uint32_t i_buffer = 0; i_buffer < cNumBuffer; ++i_buffer)
        {
            // リソースを取得してテクスチャーを生成
            Microsoft::WRL::ComPtr<ID3D12Resource> p_resource;
            if (FAILED(mpSwapChain->GetBuffer(i_buffer, IID_PPV_ARGS(&p_resource))))
            {
                BEL_ERROR_LOG("スワップチェーンのバッファー取得に失敗しました\n");
                return false;
            }

            gfx::Texture::InitializeArg init_arg;
            init_arg.width     = desc.Width;
            init_arg.height    = desc.Height;
            init_arg.format    = mIsSupportedHDR ? cHDRTextureFormat : cSDRsRGBTextureFormat;
            init_arg.dimension = gfx::TextureDimension::c2D;
            init_arg.resource_flags = gfx::ResourceFlag::cRenderTarget;
            mSwapChainTextures[i_buffer].initializeFromGPUMemory(
                init_arg,
                std::move(p_resource),
                gfx::ResourceState::cPresent
            );

            // テクスチャーからレンダーターゲットを作る
            mSwapChainRenderTargets[i_buffer].initialize();
            mSwapChainRenderTargets[i_buffer].updateTexture(mSwapChainTextures[i_buffer]);

            // レンダーターゲットからレンダーバッファーを作る
            mSwapChainRenderBuffers[i_buffer].setRenderTarget(0, mSwapChainRenderTargets[i_buffer]);
            mSwapChainRenderBuffers[i_buffer].setResolution(desc.Width, desc.Height);
        }
    }
    // バッファー番号を記録
    mSwapChainBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // テクスチャーコピーキューを生成する
    {
        mpTextureCopyQueue = std::make_unique<gfx::TextureCopyQueue>();
        if (!mpTextureCopyQueue->initialize())
        {
            BEL_ERROR_LOG("テクスチャーのコピーキューの生成に失敗しました\n");
            return false;
        }
    }

    // メッシュホルダー
    {
        if (!gfx::dev::MeshHolder::GetInstance().initialize())
        {
            BEL_ERROR_LOG("基本形状メッシュの生成に失敗しました\n");
            return false;
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::executeCommand()
{
    // コマンド実行
    {
        ID3D12CommandList* p_command_lists[1] = { &mpMainCommandLists[mCommandBufferIndex].getCommandList()};
        mpMainCommandQueue->getCommandQueue().ExecuteCommandLists(1, p_command_lists);
    }
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::waitCommandQueue()
{
    // 終了したことを知らせるフェンスをキューに入れる
    ResetEvent(mWaitFenceHandle);
    mpFence->Signal(0);
    mpMainCommandQueue->getCommandQueue().Signal(mpFence.Get(), 1);

    // フェンスが1になるまで待機
    mpFence->SetEventOnCompletion(1, mWaitFenceHandle);
    if (mpFence->GetCompletedValue() < 1)
    {
        WaitForSingleObject(mWaitFenceHandle, INFINITE);
    }
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::update()
{
    // HDR の情報を更新する
    updateHDRPaperWhite_();

    // 処理のはじめに呼ばれるので、ダイナミックテクスチャーはここで遅延解放する
    gfx::DynamicTextureResource::GetInstance().updateDelayedRelease();
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::present()
{
    mpSwapChain->Present(1, 0);

    // バッファー番号更新
    mSwapChainBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();
    mCommandBufferIndex = 1 - mCommandBufferIndex;

    // 動的デスクリプターヒープのバッファーも切り替え
    gfx::DynamicDescriptorHeap::GetInstance().swapBuffer();
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::finalize()
{
    mSwapChainTextures.reset();

    // 終了時にはすべてのリソースを解放
    gfx::DynamicTextureResource::GetInstance().release();
}
//-----------------------------------------------------------------------------
// getter
//-----------------------------------------------------------------------------
gfx::RenderTarget& GraphicsEngineD3D::getDefaultRenderTarget() const
{
    return mSwapChainRenderTargets[mSwapChainBufferIndex];
}
//-----------------------------------------------------------------------------
gfx::RenderBuffer& GraphicsEngineD3D::getDefaultRenderBuffer() const
{
    return mSwapChainRenderBuffers[mSwapChainBufferIndex];
}
//-----------------------------------------------------------------------------
uint64_t GraphicsEngineD3D::getTimestampFrequency() const
{
    uint64_t frequency;
    if (FAILED(mpMainCommandQueue->getCommandQueue().GetTimestampFrequency(&frequency)))
    {
        return 1;
    }
    return frequency;
}
//-----------------------------------------------------------------------------
// Internal
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::updateHDRPaperWhite_()
{
    if (mHDRPathInfoEnabled)
    {
        // パス情報を使って paper white を取得
        DISPLAYCONFIG_SDR_WHITE_LEVEL white_level = {};
        white_level.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_SDR_WHITE_LEVEL;
        white_level.header.size = sizeof(white_level);
        white_level.header.adapterId = mHDRPathInfo.targetInfo.adapterId;
        white_level.header.id = mHDRPathInfo.targetInfo.id;
        if (SUCCEEDED(DisplayConfigGetDeviceInfo(&white_level.header)))
        {
            mHDRPaperWhite = white_level.SDRWhiteLevel * 80.f / 1000.f;
        }
    }
}
//-----------------------------------------------------------------------------
// Accessor
//-----------------------------------------------------------------------------
gfx::CommandQueue& GraphicsEngineD3D::ApplicationAccessor::getMainCommandQueue()
{
    GraphicsEngineD3D& instance = GraphicsEngineD3D::GetInstance();
    BEL_ASSERT(instance.mpMainCommandQueue.get());
    return *instance.mpMainCommandQueue;
}
//-----------------------------------------------------------------------------
gfx::CommandList& GraphicsEngineD3D::ApplicationAccessor::getMainCommandList()
{
    GraphicsEngineD3D& instance = GraphicsEngineD3D::GetInstance();
    BEL_ASSERT(instance.mpMainCommandLists.get());
    return instance.mpMainCommandLists[instance.mCommandBufferIndex];
}
//-----------------------------------------------------------------------------
}