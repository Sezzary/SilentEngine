struct Input
{
    float4 Color : COLOR0;
};

struct Output
{
    float4 FragColor : SV_Target;
};

cbuffer UniformBlock : register(b0, space3)
{
    bool IsFastAlpha;
};

Output main(Input input)
{
    Output output;

    float alpha      = (IsFastAlpha * step(0.5f, input.Color.a)) + ((1.0f - IsFastAlpha) * input.Color.a);
    output.FragColor = float4(input.Color.rgb, alpha);
    return output;
}
