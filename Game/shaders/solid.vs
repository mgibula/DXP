#include "DXP.hlsli"
#include "solid.hlsli"

PS_in main(VS_in input)
{
	PS_in output;

	output.position = mul(mul(mul(float4(input.position, 1.f), worldMatrix), viewMatrix), projectionMatrix);
	output.uv = input.uv;

	return output;
}