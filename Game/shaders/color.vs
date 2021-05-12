#include "DXP.hlsli"
#include "color.hlsli"

PS_in main(VS_in input)
{
	PS_in output;

	float4x4 worldViewProj = mul(worldMatrix, mul(viewMatrix, projectionMatrix));
	output.position = mul(float4(input.position, 1.f), worldViewProj);
	output.color = input.color;
	output.origPosition = output.position;

	return output;
}