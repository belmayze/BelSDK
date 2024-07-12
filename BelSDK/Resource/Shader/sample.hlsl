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
    float4 color    : COLOR0;
};

//! constatnt buffer
cbuffer EnvironmentConstantBuffer : register(b0)
{
    float4x3 cViewMatrix;
    matrix   cProjectionMatrix;
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
   uint   vertex_id : SV_VertexId;
};

//! main
PS_INPUT main(VS_INPUT input)
{
    //float3 position_view = mul(float4(input.position, 1.f), cViewMatrix);

    // vertex output
    PS_INPUT output;
    output.position = float4(input.position, 1.f);
    //output.position = mul(float4(position_view, 1.f), cProjectionMatrix);

    float4 colors[3];
    colors[0] = float4(1.0, 0.1, 0.1, 1.0);
    colors[1] = float4(0.1, 1.0, 0.1, 1.0);
    colors[2] = float4(0.1, 0.1, 1.0, 1.0);
    output.color = colors[input.vertex_id % 3];

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
    PS_OUT output;
    output.color0 = float4(input.color);
    return output;
}

#endif // BEL_***_SHADER
