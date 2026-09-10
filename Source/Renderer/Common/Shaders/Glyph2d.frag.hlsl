#include "Utils/Constants.hlsli"
#include "Utils/Math.hlsli"

Texture2D<float4> Texture : register(t0, space2);
SamplerState      Sampler : register(s0, space2);

struct Input
{
    float4 Position : SV_Position;
    float2 TexCoord : TEXCOORD0;
    float4 Color    : COLOR0;
};

cbuffer PerObject : register(b0, space3)
{
    uint  HasGradient;
    float GradientUvMinY;
    float GradientUvMaxY;
};

static const float HIGHLIGHT = 248.0f / (float)Constants::UINT8_MAX;
static const float LOWLIGHT  = 167.0f / (float)Constants::UINT8_MAX;

float3 ModulateColor(float3 base, float3 tint)
{
    return saturate((base * tint) * 2.0f);
}

float4 main(Input input) : SV_Target
{
    // Sample texture.
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);

    // Compute gradient factor.
    float localY         = Math::Remap(input.TexCoord.y, GradientUvMinY, GradientUvMaxY, 0.0f, 1.0f);
    float dist           = abs(localY - 0.5f) * 2.0f;
    float gradientFactor = lerp(LOWLIGHT, HIGHLIGHT, saturate(1.0f - dist));

    // Combine color and apply gradient if active.
    float3 glyphTex   = HasGradient ? float3(gradientFactor, gradientFactor, gradientFactor) : texColor.rgb;
    float3 finalColor = ModulateColor(glyphTex, input.Color.rgb);
    // Old:
    //float3 finalColor = input.Color.rgb * texColor.rgb;
    //finalColor       *= lerp(1.0f, gradientFactor, float(HasGradient));

    // Compute final color.
    float alpha = input.Color.a * texColor.a;
    return float4(finalColor, alpha);
}
