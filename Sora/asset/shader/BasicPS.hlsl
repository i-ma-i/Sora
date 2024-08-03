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
    // Calculate diffuse reflection.
    /*
    // Reversing the sign of the inner product value of the normal and the light source vector gives the intensity of light striking the surface.
    float diffuseIntensity = dot(input.Normal, LightDirection);
    diffuseIntensity *= -1.0f;
    // Negative values are not required.
    if (diffuseIntensity < 0.0f)
    {
        diffuseIntensity = 0.0f;
    }
    */
    // Use built-in functions to simplify the calculation of diffuse reflections.
    float diffuseIntensity = saturate(-dot(input.Normal, LightDirection));
    float4 diffuseTextureColor = textureDiffuse.Sample(samplerLinear, input.Tex);
    float3 diffuse = diffuseTextureColor.xyz * LightColor * diffuseIntensity;
    
    // Calculate specular reflection.
    // directional light source vector
    float3 L = LightDirection; 
    // reflection vector
    float3 R = normalize(reflect(L, input.Normal));
    
    // Calculate the intensity of specular reflection.
    // Vector to camera position
    float3 V = normalize(CameraPosition - input.WorldPos);
    // Intensity of specular reflection
    float SpecularPower = 32.0f;
    float specularIntensity = pow(saturate(dot(R, V)), SpecularPower);
    float3 specular = LightColor * specularIntensity;

    return float4(AmbientColor + diffuse + specular, 1.0f);
}
