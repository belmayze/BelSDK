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
//! main
PS_INPUT main(uint vertex_id : SV_VertexId)
{
    // Vertex output
    PS_INPUT output;
    float2 texcoord = float2(vertex_id & 2, (vertex_id << 1) & 2);
    output.texcoord = texcoord;
    output.position = float4(texcoord.x * 2.0 - 1.0, -texcoord.y * 2.0 + 1.0, 0.0, 1.0);
    return output;
}

#elif BEL_PIXEL_SHADER
//-----------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------
//! Texture
Texture2D<float4> gTexture : register(t0);
SamplerState      gSampler : register(s0);

//! Constant Buffer
cbuffer Buffer0 : register(b0)
{
    float4 cColor;
};

//! main
float4 main(PS_INPUT input) : SV_TARGET
{
    return gTexture.Sample(gSampler, input.texcoord) * cColor;
}

#endif // BEL_***_SHADER
