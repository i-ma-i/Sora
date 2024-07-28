cbuffer Light : register(b1)
{
    // packoffsetÇÃédólÇ™ÇÊÇ≠ÇÌÇ©ÇÁÇ»Ç¢ÅB
    float3 LightPosition : packoffset(c0);
    float padding : packoffset(c1.x);
    float3 LightColor : packoffset(c1.y);
    float padding2 : packoffset(c2.x);
    float3 CameraPosition : packoffset(c2.y);
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float3 WorldPos : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
    return txDiffuse.Sample(samLinear, input.TexCoord);
}
