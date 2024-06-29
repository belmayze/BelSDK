/*!
 * @file   tone_mapping.hlsl
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */

//! Varying
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

#if BEL_VERTEX_SHADER
//-----------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------
//! Input
struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

//! main
PS_INPUT main(VS_INPUT input)
{
    // èoóÕ
    PS_INPUT output;
    output.position = float4(input.position, 1.f);
    output.texcoord = input.texcoord;
    return output;
}

#elif BEL_PIXEL_SHADER
//-----------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------
//! Texture
Texture2D    gTexture : register(t0);
SamplerState gSampler : register(s0);

//! main
float4 main(PS_INPUT input) : SV_TARGET
{
    return gTexture.Sample(gSampler, input.texcoord);
}

#endif // BEL_***_SHADER
