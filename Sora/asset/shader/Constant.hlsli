cbuffer Transform : register(b0)
{
    matrix World;
    matrix WVP;
};

cbuffer Light : register(b1)
{
    float4 LightDirection;
    float4 CameraPosition;
};
