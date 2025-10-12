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
    float Time;
};

Output main(Input input)
{
    Output output;

    float pulse      = (sin(Time * 2.0f) * 0.5f) + 0.5f;
    output.FragColor = float4(input.Color.rgb * ((0.8f + pulse) * 0.5f), input.Color.a);
    return output;
}
