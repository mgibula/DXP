
struct VS_in
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PS_in
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};