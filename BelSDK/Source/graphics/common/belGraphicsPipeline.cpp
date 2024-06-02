/*!
 * @file   belGraphicsPipeline.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsPipeline.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceShaderArchive.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Pipeline::initialize(const InitializeArg& arg, const res::ShaderArchive& shader)
{
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline;
    res::ShaderArchive::ShaderType type = shader.getShaderType();

    switch (type)
    {
        // プリミティブ系
        case res::ShaderArchive::ShaderType::VertexPixel:
        case res::ShaderArchive::ShaderType::VertexGeometryPixel:
        case res::ShaderArchive::ShaderType::Tessellation:
        case res::ShaderArchive::ShaderType::TessellationGeometry:
        {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
            desc.pRootSignature = &GraphicsEngine::GetInstance().getCommonGraphicsRootSignature();

            // ラスタライザー
            desc.RasterizerState.FillMode              = D3D12_FILL_MODE_SOLID;
            desc.RasterizerState.CullMode              = D3D12_CULL_MODE_BACK;
            desc.RasterizerState.FrontCounterClockwise = true;
            desc.RasterizerState.DepthClipEnable       = true;

            // ブレンダー
            desc.BlendState.RenderTarget[0].BlendEnable           = false;
            desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

            // デプス
            desc.DepthStencilState.DepthEnable    = true;
            desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
            desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

            // トポロジー
            desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

            // 出力フォーマット
            desc.NumRenderTargets = 1;
            desc.RTVFormats[0] = DXGI_FORMAT_R11G11B10_FLOAT;
            desc.DSVFormat     = DXGI_FORMAT_D32_FLOAT;

            // サンプル
            desc.SampleMask       = std::numeric_limits<uint32_t>::max();
            desc.SampleDesc.Count = 1;

            // 頂点シェーダーを使う場合は頂点レイアウトを作る (仮)
            std::array<D3D12_INPUT_ELEMENT_DESC, 2> input_elements;
            if (type == res::ShaderArchive::ShaderType::VertexPixel || type == res::ShaderArchive::ShaderType::VertexGeometryPixel ||
                type == res::ShaderArchive::ShaderType::Tessellation || type == res::ShaderArchive::ShaderType::TessellationGeometry)
            {
                input_elements[0].AlignedByteOffset    = 0;
                input_elements[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
                input_elements[0].InputSlot            = 0;
                input_elements[0].InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                input_elements[0].InstanceDataStepRate = 0;
                input_elements[0].SemanticIndex        = 0;
                input_elements[0].SemanticName         = "POSITION";
                
                input_elements[1].AlignedByteOffset    = 12; // vec3
                input_elements[1].Format               = DXGI_FORMAT_R32G32B32_FLOAT;
                input_elements[1].InputSlot            = 0;
                input_elements[1].InputSlotClass       = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                input_elements[1].InstanceDataStepRate = 0;
                input_elements[1].SemanticIndex        = 0;
                input_elements[1].SemanticName         = "NORMAL";

                desc.InputLayout.pInputElementDescs = input_elements.data();
                desc.InputLayout.NumElements        = static_cast<uint32_t>(input_elements.size());
            }

            // シェーダーコード
            if (type == res::ShaderArchive::ShaderType::VertexPixel || type == res::ShaderArchive::ShaderType::VertexGeometryPixel ||
                type == res::ShaderArchive::ShaderType::Tessellation || type == res::ShaderArchive::ShaderType::TessellationGeometry)
            {
                // VS
                auto [vs_size, vs_bin] = shader.getVertexBinary();
                desc.VS.BytecodeLength  = vs_size;
                desc.VS.pShaderBytecode = vs_bin;

                // PS
                auto [ps_size, ps_bin] = shader.getPixelBinary();
                desc.PS.BytecodeLength  = ps_size;
                desc.PS.pShaderBytecode = ps_bin;

                // GS
                if (type == res::ShaderArchive::ShaderType::VertexGeometryPixel || type == res::ShaderArchive::ShaderType::TessellationGeometry)
                {
                    auto [gs_size, gs_bin] = shader.getGeometryBinary();
                    desc.GS.BytecodeLength  = gs_size;
                    desc.GS.pShaderBytecode = gs_bin;
                }

                // HS / DS
                if (type == res::ShaderArchive::ShaderType::Tessellation || type == res::ShaderArchive::ShaderType::TessellationGeometry)
                {
                    auto [hs_size, hs_bin] = shader.getHullBinary();
                    desc.HS.BytecodeLength  = hs_size;
                    desc.HS.pShaderBytecode = hs_bin;

                    auto [ds_size, ds_bin] = shader.getDomainBinary();
                    desc.DS.BytecodeLength  = ds_size;
                    desc.DS.pShaderBytecode = ds_bin;
                }
            }

            // パイプライン生成
            if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&p_pipeline))))
            {
                return false;
            }

            break;
        }

        // メッシュシェーダー
        case res::ShaderArchive::ShaderType::Mesh:
        case res::ShaderArchive::ShaderType::MeshAmplification:
        {
            return false;
        }

        // コンピュート系
        case res::ShaderArchive::ShaderType::Compute:
        {
            D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
            desc.pRootSignature = &GraphicsEngine::GetInstance().getCommonComputeRootSignature();

            // シェーダー
            {
                auto [cs_size, cs_bin] = shader.getComputeBinary();
                desc.CS.BytecodeLength  = cs_size;
                desc.CS.pShaderBytecode = cs_bin;
            }

            // パイプライン生成
            if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateComputePipelineState(&desc, IID_PPV_ARGS(&p_pipeline))))
            {
                return false;
            }

            break;
        }

        // レイトレーシング系
        case res::ShaderArchive::ShaderType::Library:
        {
            return false;
        }
    }

    //
    mpPipeline = std::move(p_pipeline);

    return true;
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Pipeline::setPipeline(CommandContext& command) const
{
    command.getCommandList().SetPipelineState(mpPipeline.Get());
}
//-----------------------------------------------------------------------------
}