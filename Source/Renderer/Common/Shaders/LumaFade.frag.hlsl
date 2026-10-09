#include "Utils/Math.hlsli"

Texture2D<float4> Texture : register(t0, space2);
SamplerState      Sampler : register(s0, space2);

struct Input
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR0;
};

cbuffer PerFrame : register(b0, space3)
{
    float FadeAlpha;
    uint  IsWhite;
};

float4 main(Input input) : SV_Target
{
    float4 texColor     = Texture.Sample(Sampler, input.TexCoord);
    float3 workingColor = IsWhite ? (1.0f - texColor.rgb) : texColor.rgb;

    float  luma       = 1.0f - Math::Luma601(workingColor);
    float3 fadedColor = workingColor * Math::Remap(1.0f - FadeAlpha, luma, 1.0f, 0.0f, 1.0f);

    // Compute final color.
    float3 finalColor = IsWhite ? (1.0f - fadedColor) : fadedColor;
    return float4(finalColor, 1.0f);
}
