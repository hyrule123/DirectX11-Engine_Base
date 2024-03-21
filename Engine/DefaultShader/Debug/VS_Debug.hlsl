#include "Debug.hlsli"

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	float4 worldPosition = mul(pos, CB_Transform.World);
	float4 viewPosition = mul(worldPosition, CB_Transform.View);
	float4 ProjPosition = mul(viewPosition, CB_Transform.Projection);
    
    return ProjPosition;
}
