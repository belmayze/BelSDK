/*!
 * @file   belGraphicsPipeline.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsPipeline.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsTextureDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceShaderResource.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Pipeline::initialize(const InitializeArg& arg, const res::ShaderResource& shader)
{
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline;
    res::ShaderResource::ShaderType type = shader.getShaderType();

    switch (type)
    {
        // プリミティブ系
        case res::ShaderResource::ShaderType::VertexPixel:
        case res::ShaderResource::ShaderType::VertexGeometryPixel:
        case res::ShaderResource::ShaderType::Tessellation:
        case res::ShaderResource::ShaderType::TessellationGeometry:
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
            desc.DepthStencilState.DepthEnable    = false;
            desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
            desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

            // トポロジー
            desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

            // 出力フォーマット
            BEL_ASSERT(arg.num_render_target < cMaxRenderTargets);
            desc.NumRenderTargets = arg.num_render_target;
            for (uint32_t i = 0; i < arg.num_render_target; ++i)
            {
                desc.RTVFormats[i] = to_native(arg.render_target_formats[i]);
            }
            if (arg.depth_stencil_format != TextureFormat::cUnknown) { desc.DSVFormat = to_native(arg.depth_stencil_format); }

            // サンプル
            desc.SampleMask       = std::numeric_limits<uint32_t>::max();
            desc.SampleDesc.Count = 1;

            // 頂点シェーダーを使う場合は頂点レイアウトを作る (仮)
            std::array<D3D12_INPUT_ELEMENT_DESC, 2> input_elements;
            if (type == res::ShaderResource::ShaderType::VertexPixel || type == res::ShaderResource::ShaderType::VertexGeometryPixel ||
                type == res::ShaderResource::ShaderType::Tessellation || type == res::ShaderResource::ShaderType::TessellationGeometry)
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
            if (type == res::ShaderResource::ShaderType::VertexPixel || type == res::ShaderResource::ShaderType::VertexGeometryPixel ||
                type == res::ShaderResource::ShaderType::Tessellation || type == res::ShaderResource::ShaderType::TessellationGeometry)
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
                if (type == res::ShaderResource::ShaderType::VertexGeometryPixel || type == res::ShaderResource::ShaderType::TessellationGeometry)
                {
                    auto [gs_size, gs_bin] = shader.getGeometryBinary();
                    desc.GS.BytecodeLength  = gs_size;
                    desc.GS.pShaderBytecode = gs_bin;
                }

                // HS / DS
                if (type == res::ShaderResource::ShaderType::Tessellation || type == res::ShaderResource::ShaderType::TessellationGeometry)
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
        case res::ShaderResource::ShaderType::Mesh:
        case res::ShaderResource::ShaderType::MeshAmplification:
        {
            return false;
        }

        // コンピュート系
        case res::ShaderResource::ShaderType::Compute:
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
        case res::ShaderResource::ShaderType::Library:
        {
            return false;
        }
    }

    // デスクリプターヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> p_descriptor_heap;
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = 1;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }
    }

    //
    mpPipeline       = std::move(p_pipeline);
    mpDescriptorHeap = std::move(p_descriptor_heap);

    return true;
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Pipeline::setPipeline(CommandContext& command) const
{
    command.getCommandList().SetPipelineState(mpPipeline.Get());
    ID3D12DescriptorHeap* p_heaps[] = { mpDescriptorHeap.Get() };
    //command.getCommandList().SetDescriptorHeaps(1, p_heaps);
    //command.getCommandList().SetGraphicsRootDescriptorTable(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, mpDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}
//-----------------------------------------------------------------------------
}