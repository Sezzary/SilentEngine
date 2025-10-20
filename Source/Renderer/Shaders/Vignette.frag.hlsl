Texture2D    Channel0      : register(t0);
SamplerState SamplerState0 : register(s0);

float4 main(float2 uv : TEXCOORD0) : SV_Target
{
    float  vignette = (((16.0f * uv.x) * uv.y) * (1.0f - uv.x)) * (1.0f - uv.y);
    float3 color    = Channel0.Sample(SamplerState0, uv).xyz;
    color          *= pow(vignette, 0.3f);

    return float4(color, 1.0f);
}
