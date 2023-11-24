/*!
 * @file   belRootSignature.cpp
 * @brief
 * @author belmayze
 *
 * Copyright (c) belmayze. All rights reserved.
 */
// std
#include <array>
// bel
#include "gfx/core/belRootSignature.h"
#include "gfx/belGraphics.h"

namespace bel::gfx
{
//-----------------------------------------------------------------------------
bool RootSignature::create()
{
    // レンジ
    enum RangeType
    {
        cRangeTypeCBV,
        cRangeTypeSRV,
        cRangeTypeUAV,
        cRangeTypeSampler,
        cRangeTypeNum
    };
    std::array<D3D12_DESCRIPTOR_RANGE1, cRangeTypeNum> ranges = {};

    // CBV: 16
    {
        ranges[cRangeTypeCBV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        ranges[cRangeTypeCBV].NumDescriptors     = Graphics::cMaxDescriptorCBV;
        ranges[cRangeTypeCBV].BaseShaderRegister = 0;
        ranges[cRangeTypeCBV].RegisterSpace      = 0;
        ranges[cRangeTypeCBV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[cRangeTypeCBV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // SRV: 48
    {
        ranges[cRangeTypeSRV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
        ranges[cRangeTypeSRV].NumDescriptors     = Graphics::cMaxDescriptorSRV;
        ranges[cRangeTypeSRV].BaseShaderRegister = Graphics::cMaxDescriptorCBV;
        ranges[cRangeTypeSRV].RegisterSpace      = 0;
        ranges[cRangeTypeSRV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[cRangeTypeSRV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // UAV: 16
    {
        ranges[cRangeTypeUAV].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
        ranges[cRangeTypeUAV].NumDescriptors     = Graphics::cMaxDescriptorUAV;
        ranges[cRangeTypeUAV].BaseShaderRegister = Graphics::cMaxDescriptorCBV + Graphics::cMaxDescriptorSRV;
        ranges[cRangeTypeUAV].RegisterSpace      = 0;
        ranges[cRangeTypeUAV].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[cRangeTypeUAV].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }
    // Sampler: 16
    {
        // Sampler のデスクリプターヒープは別々なので、レンジは最初から開始される
        ranges[cRangeTypeSampler].RangeType          = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
        ranges[cRangeTypeSampler].NumDescriptors     = Graphics::cMaxDescriptorSampler;
        ranges[cRangeTypeSampler].BaseShaderRegister = 0;
        ranges[cRangeTypeSampler].RegisterSpace      = 0;
        ranges[cRangeTypeSampler].Flags              = D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
        ranges[cRangeTypeSampler].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
    }

    // デスクリプターヒープは必ず CopyDescriptors() を使う前提のシステムなので単一のヒープのみ使用する
    // ただし CBV_SRV_UAV と Sampler のデスクリプターヒープは別々になる
    enum ParamType
    {
        cParamType_CBV_SRV_UAV,
        cParamType_Sampler,
        cParamTypeNum
    };

    std::array<D3D12_ROOT_PARAMETER1, cParamTypeNum> params = {};
    {
        params[cParamType_CBV_SRV_UAV].ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[cParamType_CBV_SRV_UAV].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        params[cParamType_CBV_SRV_UAV].DescriptorTable.NumDescriptorRanges = 3;
        params[cParamType_CBV_SRV_UAV].DescriptorTable.pDescriptorRanges   = &ranges[0];
    }
    {
        params[cParamType_Sampler].ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
        params[cParamType_Sampler].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        params[cParamType_Sampler].DescriptorTable.NumDescriptorRanges = 1;
        params[cParamType_Sampler].DescriptorTable.pDescriptorRanges   = &ranges[cRangeTypeSampler];
    }

    // ルートシグネチャ作成（Graphics）
    {
        Microsoft::WRL::ComPtr<ID3D12RootSignature> p_root_signature;

        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = cParamTypeNum;
        desc.Desc_1_1.pParameters   = params.data();
        desc.Desc_1_1.Flags         = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        Microsoft::WRL::ComPtr<ID3DBlob> p_root_signature_serialized;
        Microsoft::WRL::ComPtr<ID3DBlob> p_error;
        if (FAILED(D3D12SerializeVersionedRootSignature(&desc, &p_root_signature_serialized, &p_error)))
        {
            return false;
        }
        if (FAILED(Graphics::GetInstance().getDevice().CreateRootSignature(0, p_root_signature_serialized->GetBufferPointer(), p_root_signature_serialized->GetBufferSize(), IID_PPV_ARGS(&p_root_signature))))
        {
            return false;
        }
        mpGraphicsRootSignature = std::move(p_root_signature);
    }

    // ルートシグネチャ作成（Compute）
    {
        Microsoft::WRL::ComPtr<ID3D12RootSignature> p_root_signature;

        D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc = {};
        desc.Version                = D3D_ROOT_SIGNATURE_VERSION_1_1;
        desc.Desc_1_1.NumParameters = cParamTypeNum;
        desc.Desc_1_1.pParameters   = params.data();
        desc.Desc_1_1.Flags = 
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
        if (FAILED(Graphics::GetInstance().getDevice().CreateRootSignature(0, p_root_signature_serialized->GetBufferPointer(), p_root_signature_serialized->GetBufferSize(), IID_PPV_ARGS(&p_root_signature))))
        {
            return false;
        }
        mpComputeRootSignature = std::move(p_root_signature);
    }

    return true;
}
//-----------------------------------------------------------------------------
RootSignature::RootSignature() {}
RootSignature::~RootSignature() {}
//-----------------------------------------------------------------------------
}
