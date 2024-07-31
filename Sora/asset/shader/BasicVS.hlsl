#include "Constant.hlsli"

struct VS_INPUT
{
    float3 Pos : SV_Position;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float3 WorldPos : WORLDPOS;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(float4(input.Pos, 1.0f), WVP);
    output.Normal = normalize(mul(float4(input.Normal, 0.0f), World).xyz);
    output.Tex = input.Tex;
    output.WorldPos = mul(float4(input.Pos, 1.0f), World).xyz;
    return output;
}
