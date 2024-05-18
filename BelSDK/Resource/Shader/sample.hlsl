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
   float3 position : POSITION;
   float3 normal   : NORMAL0;
};

//! main
PS_INPUT main(VS_INPUT input)
{
    float3 position_view = mul(float4(input.position, 1.f), cViewMatrix);

    // èoóÕ
    PS_INPUT output;
    output.position = mul(float4(position_view, 1.f), cProjectionMatrix);
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
    output.color0 = float4(1.f, 0.f, 0.f, 1.f);
    return output;
}

#endif // BEL_***_SHADER
