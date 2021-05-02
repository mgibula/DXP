#include "DXP.hlsli"
#include "solid.hlsli"

PS_in main(VS_in input)
{
	PS_in output;
	output.position = mul(float4(input.position, 1.f), transform);
	output.uv = input.uv;

	return output;
}