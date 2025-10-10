struct Input
{
    uint VertexIdx : SV_VertexID;
};

struct Output
{
    float4 Color    : TEXCOORD0;
    float4 Position : SV_Position;
};

Output main(Input input)
{
    Output output;
    float2 pos;

    if (input.VertexIdx == 0)
    {
        pos          = (-1.0f).xx;
        output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        if (input.VertexIdx == 1)
        {
            pos          = float2(1.0f, -1.0f);
            output.Color = float4(0.0f, 1.0f, 0.0f, 1.0f);
        }
        else
        {
            if (input.VertexIdx == 2)
            {
                pos          = float2(0.0f, 1.0f);
                output.Color = float4(0.0f, 0.0f, 1.0f, 1.0f);
            }
        }
    }

    output.Position = float4(pos, 0.0f, 1.0f);
    return output;
}
