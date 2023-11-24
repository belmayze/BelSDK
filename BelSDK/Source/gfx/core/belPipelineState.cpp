/*!
 * @file   belShader.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "gfx/core/belPipelineState.h"
#include "gfx/core/belRootSignature.h"
#include "gfx/core/belShader.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool PipelineState::initialize(const InitializeArg& arg)
{
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline_state;

    //
    Shader::Mode shader_mode = arg.mShader.getMode();

    if (shader_mode == Shader::cModeVertexPixel || shader_mode == Shader::cModeAmplificationMeshPixel)
    {
        // 頂点シェーダー、メッシュシェーダーの場合

        // ラスタライザー
        D3D12_RASTERIZER_DESC rasterizer_desc = {};
        rasterizer_desc.FillMode              = D3D12_FILL_MODE_SOLID;
        rasterizer_desc.CullMode              = D3D12_CULL_MODE_BACK;
        rasterizer_desc.FrontCounterClockwise = true;
        rasterizer_desc.DepthClipEnable       = true;

        // ブレンダー
        D3D12_BLEND_DESC blend_desc = {};
        blend_desc.RenderTarget[0].BlendEnable           = false;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        // デプステスト
        D3D12_DEPTH_STENCIL_DESC depth_stencil_desc = {};
        depth_stencil_desc.DepthEnable    = false;
        depth_stencil_desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        depth_stencil_desc.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

        // パイプライン設定
        if (shader_mode == Shader::cModeVertexPixel)
        {
            // VS / PS
            const Shader::VertexPixel& vs_ps = arg.mShader.getVertexPixel();

            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            
            // RootSignature
            desc.pRootSignature = &RootSignature::GetInstance().getGraphicsRootSignature();

            // @TODO: 頂点レイアウト
            desc.InputLayout;

            // @TODO: トポロジー
            desc.PrimitiveTopologyType;

            // @TODO: 出力フォーマット
            desc.NumRenderTargets;
            desc.RTVFormats[0];
            desc.DSVFormat;

            // シェーダー
            desc.VS.pShaderBytecode = vs_ps.mVertexShaderPtr;
            desc.VS.BytecodeLength  = vs_ps.mVertexShaderSize;
            desc.PS.pShaderBytecode = vs_ps.mPixelShaderPtr;
            desc.PS.BytecodeLength  = vs_ps.mPixelShaderSize;

            // ステート
            desc.RasterizerState   = rasterizer_desc;
            desc.BlendState        = blend_desc;
            desc.DepthStencilState = depth_stencil_desc;
            desc.SampleMask        = std::numeric_limits<uint32_t>::max();
            desc.SampleDesc.Count  = 1;

            // 作成
            if (FAILED(Graphics::GetInstance().getDevice().CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&p_pipeline_state))))
            {
                BEL_ERROR_WINDOW("GraphicsError", "グラフィックスのパイプライン生成に失敗しました");
                return false;
            }
        }
        else
        {
            // AS / MS / PS
            const Shader::AmplificationMeshPixel& as_ms_ps = arg.mShader.getAmplificationMeshPixel();

            // use d3dx12
            BEL_ERROR_WINDOW("GraphicsError", "グラフィックスのパイプライン生成に失敗しました");
            return false;
        }
    }
    else if (shader_mode == Shader::cModeCompute)
    {
        // CS
        const Shader::Compute& cs = arg.mShader.getCompute();

        D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};

        // RootSignature
        desc.pRootSignature = &RootSignature::GetInstance().getComputeRootSignature();

        // シェーダー
        desc.CS.pShaderBytecode = cs.mComputeShaderPtr;
        desc.CS.BytecodeLength  = cs.mComputeShaderSize;

        // 作成
        if (FAILED(Graphics::GetInstance().getDevice().CreateComputePipelineState(&desc, IID_PPV_ARGS(&p_pipeline_state))))
        {
            BEL_ERROR_WINDOW("GraphicsError", "コンピュートのパイプライン生成に失敗しました");
            return false;
        }
    }
    else
    {
        BEL_ERROR_WINDOW("GraphicsError", "不明なパイプラインです");
        BEL_ASSERT(0);
        return false;
    }

    // 保持
    mpPipelineState = std::move(p_pipeline_state);

    return true;
}
//-----------------------------------------------------------------------------
}
