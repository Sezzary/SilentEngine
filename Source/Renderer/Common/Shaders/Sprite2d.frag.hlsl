#include "Utils/Constants.hlsli"
#include "Utils/Math.hlsli"

Texture2D<float4> Texture      : register(t0, space2);
Texture2D<float4> PaletteAtlas : register(t1, space2);
SamplerState      Sampler      : register(s0, space2);

struct Input
{
    float4              Position   : SV_Position;
    float2              TexCoord   : TEXCOORD0;
    float4              Color      : COLOR0;
    nointerpolation int PaletteIdx : TEXCOORD1;
};

cbuffer PerObject : register(b0, space3)
{
    uint IsFastAlpha;
};

float4 main(Input input) : SV_Target
{
    // Sample texture.
    float4 texColor = Texture.Sample(Sampler, input.TexCoord);

    // Handle indexed texture.
    if (input.PaletteIdx != Constants::NO_VALUE)
    {
        // Get palette atlas dimensions.
        uint paletteWidth;
        uint paletteHeight;
        PaletteAtlas.GetDimensions(paletteWidth, paletteHeight);

        // Get color index.
        int colorIdx = Math::DecodeColorIdx(texColor);

        // Set indexed texture color.
        int2 paletteCoords = int2(colorIdx, input.PaletteIdx);
        texColor           = PaletteAtlas.Load(int3(paletteCoords, 0));
    }

    // Compute vertex and texture alpha combination.
    float alpha = input.Color.a * texColor.a;
    alpha       = lerp(alpha, step(0.5f, alpha), float(IsFastAlpha));

    // Compute final color.
    float3 finalColor = (input.Color.rgb * texColor.rgb) * input.Color.a;
    return float4(finalColor, alpha);
}
