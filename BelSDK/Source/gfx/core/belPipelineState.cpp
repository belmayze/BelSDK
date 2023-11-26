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
        {
            const PipelineStateContext::BlendDesc& desc = arg.mContext.getBlendDesc();

            for (uint32_t i = 0; i < PipelineStateContext::cMaxRenderTarget; ++i)
            {
                blend_desc.RenderTarget[i].BlendEnable           = desc.mRenderTargets[i].mEnable;
                blend_desc.RenderTarget[i].RenderTargetWriteMask = desc.mRenderTargets[i].mWriteMask;
                blend_desc.RenderTarget[i].SrcBlend              = static_cast<D3D12_BLEND>(desc.mRenderTargets[i].mSrcColor);
                blend_desc.RenderTarget[i].DestBlend             = static_cast<D3D12_BLEND>(desc.mRenderTargets[i].mDstColor);
                blend_desc.RenderTarget[i].BlendOp               = static_cast<D3D12_BLEND_OP>(desc.mRenderTargets[i].mColorOp);
                blend_desc.RenderTarget[i].SrcBlendAlpha         = static_cast<D3D12_BLEND>(desc.mRenderTargets[i].mSrcAlpha);
                blend_desc.RenderTarget[i].DestBlendAlpha        = static_cast<D3D12_BLEND>(desc.mRenderTargets[i].mDstAlpha);
                blend_desc.RenderTarget[i].BlendOpAlpha          = static_cast<D3D12_BLEND_OP>(desc.mRenderTargets[i].mAlphaOp);

            }
        }

        // デプステスト
        D3D12_DEPTH_STENCIL_DESC depth_stencil_desc = {};
        depth_stencil_desc.DepthEnable    = arg.mContext.getDepthStencilDesc().mEnableDepth;
        depth_stencil_desc.DepthWriteMask = arg.mContext.getDepthStencilDesc().mDepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
        depth_stencil_desc.DepthFunc      = static_cast<D3D12_COMPARISON_FUNC>(arg.mContext.getDepthStencilDesc().mDepthFunc);

        // パイプライン設定
        if (shader_mode == Shader::cModeVertexPixel)
        {
            // VS / PS
            const Shader::VertexPixel& vs_ps = arg.mShader.getVertexPixel();

            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            
            // RootSignature
            desc.pRootSignature = &RootSignature::GetInstance().getGraphicsRootSignature();

            // 頂点レイアウト
            std::array<D3D12_INPUT_ELEMENT_DESC, cMaxVertexAttribute> input_elements;
            for (uint32_t i = 0; i < arg.mInputContext.mNumInputLayout; ++i)
            {
                input_elements[i].SemanticName         = arg.mInputContext.mInputLayouts[i].mName.c_str();
                input_elements[i].SemanticIndex        = arg.mInputContext.mInputLayouts[i].mSemanticIndex;
                input_elements[i].Format               = arg.mInputContext.mInputLayouts[i].mFormat;
                input_elements[i].InputSlot            = arg.mInputContext.mInputLayouts[i].mSlotIndex;
                input_elements[i].AlignedByteOffset    = arg.mInputContext.mInputLayouts[i].mOffset;
                input_elements[i].InstanceDataStepRate = arg.mInputContext.mInputLayouts[i].mInstanceDataStepRate;
                input_elements[i].InputSlotClass       = input_elements[i].InstanceDataStepRate == 0 ? D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA : D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
            }
            desc.InputLayout.pInputElementDescs = input_elements.data();
            desc.InputLayout.NumElements        = arg.mInputContext.mNumInputLayout;

            // トポロジー
            desc.PrimitiveTopologyType = arg.mInputContext.mPrimitiveTopology;

            // 出力フォーマット
            desc.NumRenderTargets = arg.mOutputContext.mNumRenderTarget;
            for (uint32_t i = 0; i < arg.mOutputContext.mNumRenderTarget; ++i)
            {
                desc.RTVFormats[i] = arg.mOutputContext.mColorFormats[i];
            }
            desc.DSVFormat = arg.mOutputContext.mDepthFormat;

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
