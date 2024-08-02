#include "Constant.hlsli"

Texture2D textureDiffuse : register(t0);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float3 WorldPos : WORLDPOS;
};

float4 main(PS_INPUT input) : SV_Target
{
    float3 normal = normalize(input.Normal);
    float diffuseIntensity = saturate(dot(normal, -LightDirection));
    //float4 diffuse = textureDiffuse.Sample(samplerLinear, input.Tex) * diffuseIntensity;
    float3 diffuse = LightColor * diffuseIntensity;
    
    // 視点へのベクトル
    float3 V = normalize(CameraPosition - input.WorldPos);
    
    // 平行光源ベクトル
    float3 L = normalize(LightDirection);
    
    // 反射ベクトル
    float3 R = reflect(L, input.Normal);
    
    // 鏡面反射の強度を計算
    float SpecularPower = 32.0f;
    float specularFactor = pow(saturate(dot(V, R)), SpecularPower);
    float3 specular = LightColor * specularFactor;

    // 鏡面反射色の計算
    return float4(AmbientColor + diffuse + specular, 1.0f);
}
