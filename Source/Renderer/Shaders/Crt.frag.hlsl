// Reference: https://www.shadertoy.com/view/Ms23DR

cbuffer UniformBlock : register(b0, space3)
{
    float2 Resolution;
    float  Time;
}

Texture2D<float4> Texture : register(t0, space2);
SamplerState      Sampler : register(s0, space2);

float4 main(float2 FragCoord : SV_Position) : SV_Target
{
    float2 uv = FragCoord.xy / Resolution.xy;
    
    // Add color variation.
    float3 color;
    color.r = Texture.Sample(Sampler, float2(uv.x + 0.001f, uv.y + 0.001f)).x + 0.05f;
    color.g = Texture.Sample(Sampler, float2(uv.x,          uv.y - 0.002f)).y + 0.05f;
    color.b = Texture.Sample(Sampler, float2(uv.x - 0.002f, uv.y)).z          + 0.05f;

    // Add color variation.
    color.r += 0.08f * Texture.Sample(Sampler, (0.75f * float2(0.025f,  -0.027f)) + float2(uv.x + 0.001f, uv.y + 0.001f)).x;
    color.g += 0.05f * Texture.Sample(Sampler, (0.75f * float2(-0.022f, -0.02f))  + float2(uv.x,          uv.y - 0.002f)).y;
    color.b += 0.08f * Texture.Sample(Sampler, (0.75f * float2(-0.02f,  -0.018f)) + float2(uv.x - 0.002f, uv.y)).z;

    // Adjust color.
    color = clamp((color * 0.6f) + (((0.4f * color) * color) * 1.0f), 0.0f, 1.0f);

    // Add scan line effect.
    float scanLine = clamp(0.35f + (0.35f * sin(3.5f + ((uv.y * Resolution.y) * 1.5f))), 0.0f, 1.0f);
    scanLine       = pow(scanLineIntensity, 1.7f);
    color         *= (0.4f + (0.7f * scanLineEffect)) * 2.8f;

    // Add flicker effect.
    color *= 1.0f + (0.01f * sin(110.0f * Time));
    if (uv.x < 0.0f || uv.x > 1.0f ||
        uv.y < 0.0f || uv.y > 1.0f)
    {
        color *= 0.0f;
    }

    // Add vertical line effect.
    color *= 1.0f - (0.5f * (float3(clamp((fmod(FragCoord.x, 2.0f) - 1.0f) * 2.0f, 0.0f, 1.0f))));

    return float4(color, 1.0f);
}
