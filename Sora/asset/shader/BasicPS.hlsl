#include "Constant.hlsli"

Texture2D textureDiffuse : register(t0);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    return textureDiffuse.Sample(samplerLinear, input.Tex);
}
