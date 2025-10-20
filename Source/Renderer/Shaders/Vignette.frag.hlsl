cbuffer UniformBlock : register(b0, space3)
{
    float2 Resolution;
}

Texture2D<float4> Texture : register(t0, space2);
SamplerState      Sampler : register(s0, space2);

float4 main(float4 FragCoord : SV_Position) : SV_Target
{
    // Compute UV coordinates.
    float2 uv = FragCoord.xy / Resolution.xy;

    // Compute vignette effect.
    float vignette = (((16.0f * uv.x) * uv.y) * (1.0f - uv.x)) * (1.0f - uv.y);

    // Add vignette to texture color.
    float3 color = Texture.Sample(Sampler, uv).xyz;
    color       *= pow(vignette, 0.3f);
    return float4(color, 1.0f);
}
