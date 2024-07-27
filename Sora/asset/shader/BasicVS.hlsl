cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix WVP;
};

struct VS_INPUT
{
    float3 Pos : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(WVP, float4(input.Pos, 1.0f));
    output.Normal = input.Normal;
    output.TexCoord = input.TexCoord;
    return output;
}
