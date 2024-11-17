/*!
 * @file   sample.hlsl
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */

//! Varying
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

//! constatnt buffer
cbuffer EnvironmentConstantBuffer : register(b0)
{
    float4x3 cWorldMatrix;
    matrix   cViewProjectionMatrix;
};

#if BEL_VERTEX_SHADER
//-----------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------
//! Input
struct VS_INPUT
{
   float3 position  : POSITION;
   float3 normal    : NORMAL0;
   float2 texcoord  : TEXCOORD0;
   uint   vertex_id : SV_VertexId;
};

//! main
PS_INPUT main(VS_INPUT input)
{
    float3 position_world = mul(float4(input.position, 1.f), cWorldMatrix);

    // vertex output
    PS_INPUT output;
    output.position = mul(float4(position_world, 1.f), cViewProjectionMatrix);
    output.normal   = input.normal;
    output.texcoord = input.texcoord;

    return output;
}

#elif BEL_PIXEL_SHADER
//-----------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------
//! Output
struct PS_OUT
{
    float4 color0 : SV_TARGET;
};

//! main
PS_OUT main(PS_INPUT input)
{
    // normal
    float3 normal = normalize(input.normal);

    // texcoord
    float2 texcoord = input.texcoord;

    PS_OUT output;
    output.color0 = float4(normal * 0.5 + 0.5, 1.0);
    return output;
}

#endif // BEL_***_SHADER
