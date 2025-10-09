struct Input
{
    float4 Color : COLOR0;
};

struct Output
{
    float4 FragColor : SV_Target;
};

Output main(Input input)
{
    Output output;

    output.FragColor = input.Color;
    return output;
}
