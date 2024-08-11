/*!
 * @file   belGraphicsPipeline.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// bel
#include "graphics/common/belGraphicsConstantBuffer.h"
#include "graphics/common/belGraphicsPipeline.h"
#include "graphics/common/belGraphicsTexture.h"
#include "graphics/internal/belGraphicsGlobalDescriptorRegistry.h"
#include "graphics/belGraphicsEngine.h"
#include "resource/belResourceShaderResource.h"

namespace bel::gfx {
//-----------------------------------------------------------------------------
// initialize
//-----------------------------------------------------------------------------
bool Pipeline::initialize(const InitializeArg& arg, const res::ShaderResource& shader)
{
    Microsoft::WRL::ComPtr<ID3D12RootSignature> p_root_signature;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> p_pipeline;
    mShaderType = shader.getShaderType();

    // ルートシグネチャの設定
    std::array<D3D12_DESCRIPTOR_RANGE1, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER + 1> ranges = {};
    uint32_t num_ranges = 0;

    // SRV
    if (arg.mNumTexture > 0)
    {
        ranges[num_ranges].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        ranges[num_ranges].NumDescriptors     = arg.mNumTexture;
        ranges[num_ranges].BaseShaderRegister = 0;
        ranges[num_ranges].RegisterSpace      = 0;
        ranges[num_ranges].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[num_ranges].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        ++num_ranges;
    }
    // CBV
    if (arg.mNumConstantBuffer > 0)
    {
        ranges[num_ranges].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        ranges[num_ranges].NumDescriptors     = arg.mNumConstantBuffer;
        ranges[num_ranges].BaseShaderRegister = 0;
        ranges[num_ranges].RegisterSpace      = 0;
        ranges[num_ranges].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[num_ranges].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
        ++num_ranges;

        mConstantBufferOffset = arg.mNumTexture;
    }

    // デスクリプターヒープは必ず CopyDescriptors() を使う前提のシステムなので単一のヒープのみ使用する
    // ただし CBV_SRV_UAV と Sampler のデスクリプターヒープは別々になる
    std::array<D3D12_ROOT_PARAMETER1, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER + 1> params = {};
    {
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].DescriptorTable.NumDescriptorRanges = num_ranges;
        params[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].DescriptorTable.pDescriptorRanges   = &ranges[0];
    }

    switch (mShaderType)
    {
        // プリミティブ系
        case res::ShaderResource::ShaderType::VertexPixel:
        case res::ShaderResource::ShaderType::VertexGeometryPixel:
        case res::ShaderResource::ShaderType::Tessellation:
        case res::ShaderResource::ShaderType::TessellationGeometry:
        {
            // ルートシグネチャ
            {
                D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
                desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
                desc.Desc_1_1.NumParameters = num_ranges > 0 ? 1 : 0;
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
                if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateRootSignature(
                    0,
                    p_root_signature_serialized->GetBufferPointer(),
                    p_root_signature_serialized->GetBufferSize(),
                    IID_PPV_ARGS(&p_root_signature)
                )))
                {
                    return false;
                }
            }

            // パイプライン
            {
                D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
                desc.pRootSignature = p_root_signature.Get();

                // ラスタライザー
                desc.RasterizerState.FillMode              = D3D12_FILL_MODE_SOLID;
                desc.RasterizerState.CullMode              = D3D12_CULL_MODE_BACK;
                desc.RasterizerState.FrontCounterClockwise = true;
                desc.RasterizerState.DepthClipEnable       = true;

                // ブレンダー
                desc.BlendState.RenderTarget[0].BlendEnable           = false;
                desc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

                // デプス
                desc.DepthStencilState.DepthEnable    = arg.mDepthConfig.mDepthEnable;
                desc.DepthStencilState.DepthWriteMask = arg.mDepthConfig.mDepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
                desc.DepthStencilState.DepthFunc      = D3D12_COMPARISON_FUNC_LESS;

                // トポロジー
                desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

                // 出力フォーマット
                BEL_ASSERT(arg.mNumRenderTarget < cMaxRenderTargets);
                desc.NumRenderTargets = arg.mNumRenderTarget;
                for (uint32_t i = 0; i < arg.mNumRenderTarget; ++i)
                {
                    desc.RTVFormats[i] = to_native(arg.mRenderTargetFormats[i]);
                }
                if (arg.mDepthStencilFormat != TextureFormat::cUnknown) { desc.DSVFormat = to_native(arg.mDepthStencilFormat); }

                // サンプル
                desc.SampleMask       = std::numeric_limits<uint32_t>::max();
                desc.SampleDesc.Count = 1;

                // 頂点シェーダーを使う場合は頂点レイアウトを作る (仮)
                std::array<D3D12_INPUT_ELEMENT_DESC, 2> input_elements;
                if (mShaderType == res::ShaderResource::ShaderType::VertexPixel || mShaderType == res::ShaderResource::ShaderType::VertexGeometryPixel ||
                    mShaderType == res::ShaderResource::ShaderType::Tessellation || mShaderType == res::ShaderResource::ShaderType::TessellationGeometry)
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
                if (mShaderType == res::ShaderResource::ShaderType::VertexPixel || mShaderType == res::ShaderResource::ShaderType::VertexGeometryPixel ||
                    mShaderType == res::ShaderResource::ShaderType::Tessellation || mShaderType == res::ShaderResource::ShaderType::TessellationGeometry)
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
                    if (mShaderType == res::ShaderResource::ShaderType::VertexGeometryPixel || mShaderType == res::ShaderResource::ShaderType::TessellationGeometry)
                    {
                        auto [gs_size, gs_bin] = shader.getGeometryBinary();
                        desc.GS.BytecodeLength  = gs_size;
                        desc.GS.pShaderBytecode = gs_bin;
                    }

                    // HS / DS
                    if (mShaderType == res::ShaderResource::ShaderType::Tessellation || mShaderType == res::ShaderResource::ShaderType::TessellationGeometry)
                    {
                        auto [hs_size, hs_bin] = shader.getHullBinary();
                        desc.HS.BytecodeLength  = hs_size;
                        desc.HS.pShaderBytecode = hs_bin;

                        auto [ds_size, ds_bin] = shader.getDomainBinary();
                        desc.DS.BytecodeLength  = ds_size;
                        desc.DS.pShaderBytecode = ds_bin;
                    }
                }

                //
                if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&p_pipeline))))
                {
                    return false;
                }
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
            // ルートシグネチャ
            {
                D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
                desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
                desc.Desc_1_1.NumParameters = num_ranges > 0 ? 1 : 0;
                desc.Desc_1_1.pParameters   = params.data();
                desc.Desc_1_1.Flags         =
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
                    D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS;

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
                if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateRootSignature(
                    0,
                    p_root_signature_serialized->GetBufferPointer(),
                    p_root_signature_serialized->GetBufferSize(),
                    IID_PPV_ARGS(&p_root_signature)
                )))
                {
                    return false;
                }
            }

            // パイプライン
            {
                D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
                desc.pRootSignature = p_root_signature.Get();

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
    if (num_ranges > 0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.NumDescriptors = arg.mNumTexture + arg.mNumConstantBuffer;
        desc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        desc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

        if (FAILED(GraphicsEngine::GetInstance().getDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(&p_descriptor_heap))))
        {
            return false;
        }
    }

    //
    mpRootSignature  = std::move(p_root_signature);
    mpPipeline       = std::move(p_pipeline);
    mpDescriptorHeap = std::move(p_descriptor_heap);

    return true;
}
//-----------------------------------------------------------------------------
// shader
//-----------------------------------------------------------------------------
void Pipeline::activateTexture(uint32_t index, const Texture& texture) const
{
    D3D12_CPU_DESCRIPTOR_HANDLE src = GlobalDescriptorRegistry::GetInstance().getDescriptorHandle(texture.getDescriptorHandle());
    D3D12_CPU_DESCRIPTOR_HANDLE dst = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    dst.ptr += GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * index;

    GraphicsEngine::GetInstance().getDevice().CopyDescriptors(
        1, &dst, nullptr,
        1, &src, nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );
}
//-----------------------------------------------------------------------------
void Pipeline::activateConstantBuffer(uint32_t index, const ConstantBuffer& buffer) const
{
    D3D12_CPU_DESCRIPTOR_HANDLE src = GlobalDescriptorRegistry::GetInstance().getDescriptorHandle(buffer.getCurrentDescriptorHandle());
    D3D12_CPU_DESCRIPTOR_HANDLE dst = mpDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
    dst.ptr += GraphicsEngine::GetInstance().getDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * (mConstantBufferOffset + index);

    GraphicsEngine::GetInstance().getDevice().CopyDescriptors(
        1, &dst, nullptr,
        1, &src, nullptr,
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
    );
}
//-----------------------------------------------------------------------------
// command
//-----------------------------------------------------------------------------
void Pipeline::setPipeline(CommandContext& command) const
{
    // ルートシグネチャ
    if (mShaderType == res::ShaderResource::ShaderType::Compute)
    {
        command.getCommandList().SetComputeRootSignature(mpRootSignature.Get());
    }
    else
    {
        command.getCommandList().SetGraphicsRootSignature(mpRootSignature.Get());
    }

    // パイプライン
    command.getCommandList().SetPipelineState(mpPipeline.Get());

    // デスクリプターヒープがあれば適用
    if (mpDescriptorHeap.Get())
    {
        ID3D12DescriptorHeap* p_heaps[] = { mpDescriptorHeap.Get() };
        command.getCommandList().SetDescriptorHeaps(1, p_heaps);
        command.getCommandList().SetGraphicsRootDescriptorTable(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, mpDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    }
}
//-----------------------------------------------------------------------------
}