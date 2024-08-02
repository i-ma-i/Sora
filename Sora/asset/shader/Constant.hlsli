cbuffer Transform : register(b0)
{
    matrix World;
    matrix WVP;
};

cbuffer Camera : register(b1)
{
    float3 CameraPosition;
    float PaddingCameraPosition;
};

cbuffer DirectionalLight : register(b2)
{
    float3 LightDirection;
    float PaddingLightDirection;
    float3 LightColor;
    float PaddingLightColor;
    float3 AmbientColor;
    float PaddingAmbientColor;
};
