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
    uint  cDisplayMappingType;
    uint  cToneMappingType;
    float cHDRPaperWhite;
};
#define DISPLAY_MAPPING_TYPE_SRGB   ( 0 )
#define DISPLAY_MAPPING_TYPE_ST2084 ( 1 )

#define TONE_MAPPING_TYPE_LINEAR ( 0 )
#define TONE_MAPPING_TYPE_FILMIC ( 1 )

//! カラースペース変換
float3 srgbTorec2020( float3 srgb )
{
    return float3(
        dot( float3( 0.6274038959, 0.3292830384, 0.0433130657 ), srgb ),
        dot( float3( 0.0690972894, 0.9195403951, 0.0113623156 ), srgb ),
        dot( float3( 0.0163914389, 0.0880133079, 0.8955952532 ), srgb )
    );
}
float3 rec2020Tosrgb( float3 rec2020 )
{
    return float3(
        dot( float3(  1.6604910021, -0.5876411388, -0.0728498633 ), rec2020 ),
        dot( float3( -0.1245504745,  1.1328998971, -0.0083494226 ), rec2020 ),
        dot( float3( -0.0181507634, -0.1005788980,  1.1187296614 ), rec2020 )
    );
}

//! luminance
float calcLuminanceRec2020( float3 color )
{
    return dot( float3( 0.2627002120, 0.6779980715, 0.0593017165 ), color );
}

//! ACES Film
float3 acesFilm( float3 x )
{
    // https://knarkowicz.wordpress.com/2016/08/31/hdr-display-first-steps/
    float a = 15.8;
    float b = 2.12;
    float c = 1.2;
    float d = 5.92;
    float e = 1.9;
    return ( x * ( a * x + b ) ) / ( x * ( c * x + d ) + e );
}

//! reinhard
float reinhard( float x, float paper_white, float max_luminance )
{
    float k = max_luminance / ( paper_white - max_luminance / paper_white );
    return k * x / ( k + x );
}

//! ST2084
float3 linearToSt2084( float3 color )
{
    const float cM1 = 2610.0 / 4096.0 / 4.0;
    const float cM2 = 2523.0 / 4096.0 * 128.0;
    const float cC1 = 3424.0 / 4096.0;
    const float cC2 = 2413.0 / 4096.0 * 32.0;
    const float cC3 = 2392.0 / 4096.0 * 32.0;
    const float cLuminanceMax = 10000.0;

    float3 np = pow( color / cLuminanceMax, cM1 );
    float3 numerator   = cC1 + cC2 * np;
    float3 denominator = 1.0 + cC3 * np;
    return pow( numerator / denominator, cM2 );
}

//! main
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 row_color = gTexture.Sample(gSampler, input.texcoord);

    // 色域拡張
    float3 input_color = srgbTorec2020( row_color.rgb );
    
    // RRT (Reference Rendering Transform)
    float3 tone_mapped_color;
    switch (cToneMappingType)
    {
        case TONE_MAPPING_TYPE_LINEAR:
            tone_mapped_color = input_color;
            break;

        case TONE_MAPPING_TYPE_FILMIC:
            tone_mapped_color = acesFilm(input_color);
            break;
    }

    // ODT (Output Device Transform)
    if ( cDisplayMappingType == DISPLAY_MAPPING_TYPE_SRGB )
    {
        // sRGB なら色域を sRGB に戻す
        // 出力バッファーが sRGB ガンマ想定なので、ガンマはリニアで出力
        return float4( rec2020Tosrgb( tone_mapped_color ), row_color.a );
    }
    else
    {
        // HDR なら色域を Rec.2020 のまま InvEOTF[PQ] のガンマ変換を掛ける
        return float4( linearToSt2084( tone_mapped_color * cHDRPaperWhite ), row_color.a );
    }

    //
    return float4( 0.0, 0.0, 0.0, 1.0 );
}

#endif // BEL_***_SHADER
