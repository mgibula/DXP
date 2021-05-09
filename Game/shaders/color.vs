#include "DXP.hlsli"
#include "color.hlsli"

PS_in main(VS_in input)
{
	PS_in output;

	output.position = mul(mul(mul(float4(input.position, 1.f), worldMatrix), viewMatrix), projectionMatrix);
	output.color = input.color;

	return output;
}