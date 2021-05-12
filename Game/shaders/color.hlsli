
struct VS_in
{
    float3 position : POSITION;
    float3 color : COLOR;
};

struct PS_in
{
    float4 position : SV_POSITION;
    float3 color: COLOR;
    float4 origPosition : POSITION;
};