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

//! main
float4 main(PS_INPUT input) : SV_TARGET
{
    float4 input_color = gTexture.Sample(gSampler, input.texcoord);

    // 仮
#if 0
    {
        float val = input.texcoord.x;

        if      ( input.texcoord.y < 0.05 ) { input_color.rgb = float3( val, 0.0, 0.0 ); }
        else if ( input.texcoord.y < 0.10 ) { input_color.rgb = float3( val * 10.0, 0.0, 0.0 ); }
        else if ( input.texcoord.y < 0.15 ) { input_color.rgb = float3( 0.0, val, 0.0 ); }
        else if ( input.texcoord.y < 0.20 ) { input_color.rgb = float3( 0.0, val * 10.0, 0.0 ); }
        else if ( input.texcoord.y < 0.25 ) { input_color.rgb = float3( 0.0, 0.0, val ); }
        else if ( input.texcoord.y < 0.30 ) { input_color.rgb = float3( 0.0, 0.0, val * 10.0 ); }
        else if ( input.texcoord.y < 0.35 ) { input_color.rgb = float3( val, val, 0.0 ); }
        else if ( input.texcoord.y < 0.40 ) { input_color.rgb = float3( val * 10.0, val * 10.0, 0.0 ); }
        else if ( input.texcoord.y < 0.45 ) { input_color.rgb = float3( 0.0, val, val ); }
        else if ( input.texcoord.y < 0.50 ) { input_color.rgb = float3( 0.0, val * 10.0, val * 10.0 ); }
        else if ( input.texcoord.y < 0.55 ) { input_color.rgb = float3( val, 0.0, val ); }
        else if ( input.texcoord.y < 0.60 ) { input_color.rgb = float3( val * 10.0, 0.0, val * 10.0 ); }
        else if ( input.texcoord.y < 0.65 ) { input_color.rgb = float3( val, val * 0.5, val * 0.25 ); }
        else if ( input.texcoord.y < 0.70 ) { input_color.rgb = float3( val * 10.0, val * 5.0, val * 2.5 ); }
        else if ( input.texcoord.y < 0.75 ) { input_color.rgb = float3( val * 0.25, val, val * 0.5 ); }
        else if ( input.texcoord.y < 0.80 ) { input_color.rgb = float3( val * 2.5, val * 10.0, val * 5.0 ); }
        else if ( input.texcoord.y < 0.85 ) { input_color.rgb = float3( val * 0.5, val * 0.25, val ); }
        else if ( input.texcoord.y < 0.90 ) { input_color.rgb = float3( val * 5.0, val * 2.5, val * 10.0 ); }
        else if ( input.texcoord.y < 0.95 ) { input_color.rgb = float3( val, val, val ); }
        else if ( input.texcoord.y < 1.00 ) { input_color.rgb = float3( val * 10.0, val * 10.0, val * 10.0 ); }
    }
#endif

    // カラー拡張
    float3 pre_tonemapped_color = srgbTorec2020( input_color.rgb );
    
    // RRT (Reference Rendering Transform)
    float3 tonemapped_color = acesFilm( pre_tonemapped_color );

    // ODT (Output Device Transform)
    {
        float luminance = calcLuminanceRec2020( tonemapped_color );
        float tone_scale = reinhard( luminance, 80.0, 80.0 );
        tonemapped_color = tonemapped_color * tone_scale / luminance;
    }

    //
    return float4( rec2020Tosrgb( tonemapped_color ), input_color.a );
}

#endif // BEL_***_SHADER
