// Reference: https://www.shadertoy.com/view/Ms23DR

cbuffer PostProcessUniforms : register(b0)
{
    float2 Resolution;
    float  Time;
    bool   HasVignette;
}

Texture2D    Channel0        : register(t0);
SamplerState Channel0Sampler : register(s0);

float4 main(float2 fragCoord : SV_Position) : SV_Target
{
    float2 uv = fragCoord.xy / Resolution.xy;
    float  x  = 0.0f;
    
    // Add color variation.
    float3 color;
    color.r = Channel0.Sample(Channel0Sampler, float2((x + uv.x) + 0.001f, uv.y + 0.001f)).x + 0.05f;
    color.g = Channel0.Sample(Channel0Sampler, float2((x + uv.x),          uv.y - 0.002f)).y + 0.05f;
    color.b = Channel0.Sample(Channel0Sampler, float2((x + uv.x) - 0.002f, uv.y)).z          + 0.05f;

    // Add color variation.
    color.r += 0.08f * Channel0.Sample(Channel0Sampler, (0.75f * float2(x + 0.025f, -0.027f)) + float2(uv.x + 0.001f, uv.y + 0.001f)).x;
    color.g += 0.05f * Channel0.Sample(Channel0Sampler, (0.75f * float2(x - 0.022f, -0.02f))  + float2(uv.x + 0.000f, uv.y - 0.002f)).y;
    color.b += 0.08f * Channel0.Sample(Channel0Sampler, (0.75f * float2(x - 0.02f,  -0.018f)) + float2(uv.x - 0.002f, uv.y + 0.000f)).z;

    // Adjust color.
    color = clamp((color * 0.6f) + (((0.4f * color) * color) * 1.0f), 0.0f, 1.0f);

    // Optionally apply vignette.
    float vignette = (((16.0f * uv.x) * uv.y) * (1.0f - uv.x)) * (1.0f - uv.y);
    color         *= (HasVignette * pow(vignette, 0.3f)) + (1.0f - HasVignette);

    // Apply scan lines.
    float scanLineIntensity = clamp(0.35f + (0.35f * sin(3.5f + ((uv.y * Resolution.y) * 1.5f))), 0.0f, 1.0f);
    float scanLineEffect    = pow(scanLineIntensity, 1.7f);
    color                  *= 0.4f + (0.7f * scanLineEffect);
    color                  *= 2.8f;

    // Apply subtle color flicker.
    color *= 1.0f + (0.01f * sin(110.0f * Time));
    if (uv.x < 0.0f || uv.x > 1.0f ||
        uv.y < 0.0f || uv.y > 1.0f)
    {
        color *= 0.0f;
    }

    // Add vertical lines.
    color *= 1.0f - (0.65f * (float3(clamp((fmod(fragCoord.x, 2.0f) - 1.0f) * 2.0f, 0.0f, 1.0f))));

    return float4(color, 1.0f);
}
