/*!
 * @file   belGraphicsEngineD3D.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "base/belApplicationWindow.h"
#include "graphics/common/belGraphicsCommandList.h"
#include "graphics/common/belGraphicsCommandQueue.h"
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"
#include "graphics/belGraphicsEngineD3D.h"

namespace bel {
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
GraphicsEngineD3D::GraphicsEngineD3D()
{
    // グラフィックスのシングルトンはここで生成する
    gfx::TextureDescriptorRegistry::GetInstance();
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
        D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_12_0;
        Microsoft::WRL::ComPtr<IDXGIAdapter1> p_adapter;
        Microsoft::WRL::ComPtr<ID3D12Device6> p_device;
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

            if (SUCCEEDED(D3D12CreateDevice(p_adapter.Get(), feature_level, IID_PPV_ARGS(&p_device))))
            {
                found_adapter = true;
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

        mpMainCommandList = std::make_unique<gfx::CommandList>();
        if (!mpMainCommandList->initialize(gfx::CommandType::cGraphics))
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

    // スワップチェーン
    {
        DXGI_SWAP_CHAIN_DESC1 desc = {};
        desc.Width              = ApplicationWindow::GetInstance().getRenderWidth();
        desc.Height             = ApplicationWindow::GetInstance().getRenderHeight();
        desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
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
        mpSwapChain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709);
    }

    // グローバルなテクスチャーデスクリプターを作る
    if (!gfx::TextureDescriptorRegistry::GetInstance().allocate(cMaxTextureHandle))
    {
        BEL_PRINT("テクスチャー用デスクリプターヒープの作成に失敗しました\n");
        return false;
    }

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
    mpMainCommandList->begin();
    {

    }
    mpMainCommandList->end();
    ID3D12CommandList* p_command_lists[1] = { &mpMainCommandList->getCommandList() };
    mpMainCommandQueue->getCommandQueue().ExecuteCommandLists(1, p_command_lists);

    // 終了したことを知らせるフェンスを最後に入れる
    ResetEvent(mWaitFenceHandle);
    mpFence->Signal(0);
    mpMainCommandQueue->getCommandQueue().Signal(mpFence.Get(), 1);
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::waitCommandQueue()
{
    // フェンスが1になるまで待機
    mpFence->SetEventOnCompletion(1, mWaitFenceHandle);
    if (mpFence->GetCompletedValue() < 1)
    {
        WaitForSingleObject(mWaitFenceHandle, INFINITE);
    }
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::present()
{
    mpSwapChain->Present(1, 0);
}
//-----------------------------------------------------------------------------
void GraphicsEngineD3D::finalize()
{
}
//-----------------------------------------------------------------------------
// internal
//-----------------------------------------------------------------------------
bool GraphicsEngineD3D::createRootSignature_()
{
    // 最大数を格納できるデスクリプターテーブルを作っておく
    enum class RangeType
    {
        cCBV,
        cSRV,
        cUAV,
        cSampler,
        cNum
    };
    std::array<D3D12_DESCRIPTOR_RANGE1, to_underlying(RangeType::cNum)> ranges = {};

    // CBV
    {
        ranges[to_underlying(RangeType::cCBV)].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        ranges[to_underlying(RangeType::cCBV)].NumDescriptors     = cMaxDescriptorCBV;
        ranges[to_underlying(RangeType::cCBV)].BaseShaderRegister = 0;
        ranges[to_underlying(RangeType::cCBV)].RegisterSpace      = 0;
        ranges[to_underlying(RangeType::cCBV)].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[to_underlying(RangeType::cCBV)].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // SRV
    {
        ranges[to_underlying(RangeType::cSRV)].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        ranges[to_underlying(RangeType::cSRV)].NumDescriptors     = cMaxDescriptorSRV;
        ranges[to_underlying(RangeType::cSRV)].BaseShaderRegister = cMaxDescriptorCBV;
        ranges[to_underlying(RangeType::cSRV)].RegisterSpace      = 0;
        ranges[to_underlying(RangeType::cSRV)].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[to_underlying(RangeType::cSRV)].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // UAV
    {
        ranges[to_underlying(RangeType::cUAV)].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        ranges[to_underlying(RangeType::cUAV)].NumDescriptors     = cMaxDescriptorUAV;
        ranges[to_underlying(RangeType::cUAV)].BaseShaderRegister = cMaxDescriptorCBV + cMaxDescriptorSRV;
        ranges[to_underlying(RangeType::cUAV)].RegisterSpace      = 0;
        ranges[to_underlying(RangeType::cUAV)].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[to_underlying(RangeType::cUAV)].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // Sampler
    {
        // Sampler のデスクリプターヒープは別々なので、レンジは最初から開始される
        ranges[to_underlying(RangeType::cSampler)].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        ranges[to_underlying(RangeType::cSampler)].NumDescriptors     = cMaxDescriptorSampler;
        ranges[to_underlying(RangeType::cSampler)].BaseShaderRegister = 0;
        ranges[to_underlying(RangeType::cSampler)].RegisterSpace      = 0;
        ranges[to_underlying(RangeType::cSampler)].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[to_underlying(RangeType::cSampler)].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }

    // デスクリプターヒープは必ず CopyDescriptors() を使う前提のシステムなので単一のヒープのみ使用する
    // ただし CBV_SRV_UAV と Sampler のデスクリプターヒープは別々になる
    enum class ParamType
    {
        cCBV_SRV_UAV,
        cSampler,
        cNum
    };

    std::array<D3D12_ROOT_PARAMETER1, to_underlying(ParamType::cNum)> params = {};
    {
        params[to_underlying(ParamType::cCBV_SRV_UAV)].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[to_underlying(ParamType::cCBV_SRV_UAV)].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
        params[to_underlying(ParamType::cCBV_SRV_UAV)].DescriptorTable.NumDescriptorRanges = 3;
        params[to_underlying(ParamType::cCBV_SRV_UAV)].DescriptorTable.pDescriptorRanges   = &ranges[0];
    }
    {
        params[to_underlying(ParamType::cSampler)].ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[to_underlying(ParamType::cSampler)].ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
        params[to_underlying(ParamType::cSampler)].DescriptorTable.NumDescriptorRanges = 1;
        params[to_underlying(ParamType::cSampler)].DescriptorTable.pDescriptorRanges   = &ranges[to_underlying(RangeType::cSampler)];
    }

    // グラフィックス用ルートシグネチャ
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_graphics_root_signature;
    {
        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = to_underlying(ParamType::cNum);
        desc.Desc_1_1.pParameters   = params.data();
        desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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
        desc.Desc_1_1.NumParameters = to_underlying(ParamType::cNum);
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