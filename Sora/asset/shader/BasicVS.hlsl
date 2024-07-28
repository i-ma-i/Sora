cbuffer Transform : register(b0)
{
    matrix World;
    matrix WVP;
};

struct VS_INPUT
{
    float3 Pos : SV_Position;
    float4 Color : COLOR;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(WVP, float4(input.Pos, 1.0f));
    output.Color = input.Color;
    return output;
}
