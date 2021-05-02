cbuffer CB_Transform : register(b0)
{
    float4x4 transform;
};

cbuffer CB_PerObject: register(b4)
{
    float offsetX;
    float offsetY;
};

SamplerState pointSampler : register(s0);
SamplerState bilinearSampler : register(s1);
SamplerState trilinearSampler : register(s2);
SamplerState anisotropicSampler : register(s3);
