/*!
 * @file   debug_text.hlsl
 * @brief  
 * @author belmayze
 * 
 * Copyright (c) belmayze. All rights reserved.
 */

//! Attribute
struct VS_INPUT
{
    float3 position    : POSITION;
    float2 texcoord    : TEXCOORD;
    uint   instance_id : SV_InstanceID;
};

//! Varying
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

//! constatnt buffer
struct InstanceProperty
{
    float2 position;
    float2 uv_offset;
    float2 uv_scale;
    float2 size;
};
cbuffer InstanceConstantBuffer : register(b0)
{
    float cScale;
    InstanceProperty cInstanceProperties[1024];
};

#if BEL_VERTEX_SHADER
//-----------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------
//! main
PS_INPUT main(VS_INPUT input)
{
    // property
    InstanceProperty property = cInstanceProperties[input.instance_id];

    // 四角形ポリゴンをサイズに合わせる
    float2 position_local;
    position_local.x = input.position.x * (9.0 / 16.0) * cScale * property.size.x;
    position_local.y = input.position.y * cScale * property.size.y;

    // Vertex output
    PS_INPUT output;
    output.position.xy = position_local + property.position;
    output.position.z  = 0.0;
    output.position.w  = 1.0;

    output.texcoord = input.texcoord * property.uv_scale + property.uv_offset;
    return output;
}

#elif BEL_PIXEL_SHADER
//-----------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------
//! Texture
Texture2D<float4> gTexture : register(t0);
SamplerState      gSampler : register(s0);

//! main
float4 main(PS_INPUT input) : SV_TARGET
{
    float color = gTexture.Sample(gSampler, input.texcoord).r;
    if (color > 0.5)
    {
        discard;
    }
    return float4(1.0, 1.0, 1.0, 1.0);
}

#endif // BEL_***_SHADER
