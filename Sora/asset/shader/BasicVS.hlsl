cbuffer Transform : register(b0)
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
    float3 WorldPos : TEXCOORD1;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(WVP, float4(input.Pos, 1.0f));
    // positionÇ∆ìØÇ∂ÇÊÇ§Ç…ç¿ïWïœä∑Ç≈Ç´Ç»Ç¢ÅH
    float3x3 World3x3 = (float3x3)World;
    output.Normal = normalize(mul(World3x3, input.Normal));
    output.TexCoord = input.TexCoord;
    output.WorldPos = mul(World, float4(input.Pos, 1.0f)).xyz;
    return output;
}
