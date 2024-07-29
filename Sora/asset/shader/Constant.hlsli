cbuffer Transform : register(b0)
{
    matrix World;
    matrix WVP;
};

cbuffer Light : register(b1)
{
    float3 LightDir;
    float dirPadding;
};
