#include "Transform.hlsli"

float4 main( float2 pos : POSITION ) : SV_POSITION
{
	return float4(pos.x + offsetX, pos.y + offsetY, 0.f, 1.f);

	// return pos;
	// return float4(pos, 1.f);
}