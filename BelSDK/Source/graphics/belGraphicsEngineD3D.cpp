/*!
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
#include "graphics/common/belGraphicsRenderBuffer.h"
#include "graphics/common/belGraphicsRenderTarget.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsDynamicDescriptorHeap.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
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
        BEL_PRINT("DXGIの生成に失敗しました\n");
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
            }
        }

        if (!found_adapter)
        {
            BEL_PRINT("DirectX のバージョンが不足しています\n");
            return false;
        }
        mpDevice = std::move(p_device);
    }

    // メインのコマンドキューとコマンドリストを作る
    {
        mpMainCommandQueue = std::make_unique<gfx::CommandQueue>();
        if (!mpMainCommandQueue->initialize(gfx::CommandType::cGraphics))
        {
            BEL_PRINT("メインコマンドキューの生成に失敗しました\n");
            return false;
        }
    }
    mpMainCommandLists  = std::make_unique<gfx::CommandList[]>(2);
    for (uint32_t i_buffer = 0; i_buffer < 2; ++i_buffer)
    {
        if (!mpMainCommandLists[i_buffer].initialize(gfx::CommandType::cGraphics))
        {
            BEL_PRINT("メインコマンドリストの生成に失敗しました\n");
            return false;
        }
    }

    // メインのコマンドキューを待機するフェンス
    {
        // フェンスを待機するハンドル
        mWaitFenceHandle = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        if (!mWaitFenceHandle)
        {
            BEL_PRINT("フェンス用イベントの生成に失敗しました\n");
            return false;
        }

        // メインのキュー実行完了チェック用フェンス
        if (FAILED(mpDevice->CreateFence(1, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mpFence))))
        {
            BEL_PRINT("メインキュー用のフェンス生成に失敗しました\n");
            return false;
        }
    }

    // グローバルなデスクリプターを作る
    if (!gfx::GlobalDescriptorRegistry::GetInstance().initialize(cMaxTextureHandle))
    {
        BEL_PRINT("グローバルデスクリプターヒープの作成に失敗しました\n");
        return false;
    }

    // 動的確保を行うデスクリプターを作る
    if (!gfx::DynamicDescriptorHeap::GetInstance().initialize(cMaxDynamicDescriptorHandle))
    {
        BEL_PRINT("ダイナミックデスクリプターヒープの作成に失敗しました\n");
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
            BEL_PRINT("画面出力設定に失敗しました\n");
            return false;
        }

        if (FAILED(p_tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&mpSwapChain))))
        {
            BEL_PRINT("画面出力のインターフェース取得に失敗しました\n");
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
                BEL_PRINT("スワップチェーンのバッファー取得に失敗しました\n");
                return false;
            }

            gfx::Texture::InitializeArg init_arg;
            init_arg.mWidth     = desc.Width;
            init_arg.mHeight    = desc.Height;
            init_arg.mFormat    = mIsSupportedHDR ? cHDRTextureFormat : cSDRsRGBTextureFormat;
            init_arg.mDimension = gfx::TextureDimension::c2D;
            mSwapChainTextures[i_buffer].initializeFromGPUMemory(
                init_arg,
                std::move(p_resource),
                gfx::ResourceState::cPresent
            );

            // テクスチャーからレンダーターゲットを作る
            mSwapChainRenderTargets[i_buffer].initialize(mSwapChainTextures[i_buffer]);

            // レンダーターゲットからレンダーバッファーを作る
            mSwapChainRenderBuffers[i_buffer].setRenderTarget(0, mSwapChainRenderTargets[i_buffer]);
            mSwapChainRenderBuffers[i_buffer].setResolution(desc.Width, desc.Height);
        }
    }
    // バッファー番号を記録
    mSwapChainBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();

    // 共通のシグネチャを作る
    if (!createRootSignature_())
    {
        BEL_PRINT("ルートシグネチャの作成に失敗しました\n");
        return false;
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
void GraphicsEngineD3D::makeInitialCommand(gfx::CommandContext& command) const
{
    // 処理前にコマンドに積む必要のあるものをここで積む
    //command.getCommandList().SetGraphicsRootSignature(mpGraphicsRootSignature.Get());
    //command.getCommandList().SetComputeRootSignature(mpComputeRootSignature.Get());
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

    // バッファー番号更新
    mSwapChainBufferIndex = mpSwapChain->GetCurrentBackBufferIndex();
    mCommandBufferIndex   = 1 - mCommandBufferIndex;

    // 動的デスクリプターヒープのバッファーも切り替え
    gfx::DynamicDescriptorHeap::GetInstance().swapBuffer();
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::present()
{
    mpSwapChain->Present(1, 0);
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::finalize()
{
    mSwapChainTextures.reset();
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
// internal
//-----------------------------------------------------------------------------
bool GraphicsEngineD3D::createRootSignature_()
{
    // 最大数を格納できるデスクリプターテーブルを作っておく
    std::array<D3D12_DESCRIPTOR_RANGE1, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER + 1> ranges = {};
    
    // SRV
    {
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].NumDescriptors     = 1;//cMaxDescriptorSRV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].BaseShaderRegister = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].RegisterSpace      = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SRV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // UAV
    {
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].NumDescriptors     = cMaxDescriptorUAV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].BaseShaderRegister = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].RegisterSpace      = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_UAV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // CBV
    {
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].NumDescriptors     = cMaxDescriptorCBV;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].BaseShaderRegister = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].RegisterSpace      = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_CBV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // Sampler
    {
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].NumDescriptors     = cMaxDescriptorSampler;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].BaseShaderRegister = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].RegisterSpace      = 0;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }

    // デスクリプターヒープは必ず CopyDescriptors() を使う前提のシステムなので単一のヒープのみ使用する
    // ただし CBV_SRV_UAV と Sampler のデスクリプターヒープは別々になる
    std::array<D3D12_ROOT_PARAMETER1, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1> params = {};
    {
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].DescriptorTable.NumDescriptorRanges = 1;//3;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].DescriptorTable.pDescriptorRanges   = &ranges[0];
    }
    {
        params[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER].DescriptorTable.NumDescriptorRanges = 1;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER].DescriptorTable.pDescriptorRanges   = &ranges[D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER];
    }

    // グラフィックス用ルートシグネチャ
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_graphics_root_signature;
    {
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = 1;//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1;
        desc.Desc_1_1.pParameters   = params.data();
        desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // 固定サンプラー仮追加
        D3D12_STATIC_SAMPLER_DESC sampler_desc = {};
        sampler_desc.Filter           = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
        sampler_desc.AddressU         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressV         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.AddressW         = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        sampler_desc.ComparisonFunc   = D3D12_COMPARISON_FUNC_NEVER;
        sampler_desc.MaxLOD           = std::numeric_limits<float>::max();
        sampler_desc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        desc.Desc_1_1.NumStaticSamplers = 1;
        desc.Desc_1_1.pStaticSamplers   = &sampler_desc;

        Microsoft::WRL::ComPtr<ID3DBlob> p_root_signature_serialized;
        Microsoft::WRL::ComPtr<ID3DBlob> p_error;
        if (FAILED(D3D12SerializeVersionedRootSignature(&desc, &p_root_signature_serialized, &p_error)))
        {
            return false;
        }
        if (FAILED(mpDevice->CreateRootSignature(
            0,
            p_root_signature_serialized->GetBufferPointer(),
            p_root_signature_serialized->GetBufferSize(),
            IID_PPV_ARGS(&p_graphics_root_signature)
        )))
        {
            return false;
        }
    }

    // 頂点レイアウトを使用しないルートシグネチャ
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_compute_root_signature;
    {
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1;
        desc.Desc_1_1.pParameters   = params.data();
        desc.Desc_1_1.Flags         =
            D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
            D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;

        Microsoft::WRL::ComPtr<ID3DBlob> p_root_signature_serialized;
        Microsoft::WRL::ComPtr<ID3DBlob> p_error;
        if (FAILED(D3D12SerializeVersionedRootSignature(&desc, &p_root_signature_serialized, &p_error)))
        {
            return false;
        }
        if (FAILED(mpDevice->CreateRootSignature(
            0,
            p_root_signature_serialized->GetBufferPointer(),
            p_root_signature_serialized->GetBufferSize(),
            IID_PPV_ARGS(&p_compute_root_signature)
        )))
        {
            return false;
        }
    }

    // 成功したらシグネチャを記録する
    mpGraphicsRootSignature = std::move(p_graphics_root_signature);
    mpComputeRootSignature  = std::move(p_compute_root_signature);

    return true;
}
//-----------------------------------------------------------------------------
}