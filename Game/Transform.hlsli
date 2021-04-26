cbuffer CB_Transform : register(b0)
{
    float4x4 transform;
};

cbuffer CB_PerObject: register(b4)
{
    float offsetX;
    float offsetY;
};
