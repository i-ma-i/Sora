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
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(WVP, float4(input.Pos, 1.0f));
    output.Normal = input.Normal;
    output.Tex = input.Tex;
    return output;
}
