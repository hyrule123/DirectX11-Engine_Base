#ifndef USER_INTERFACE_HLSLI
#define USER_INTERFACE_HLSLI

#ifndef __cplusplus
#include "Engine/DefaultShader/Commons.hlsli"
struct VSIn
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
	
	uint instance_ID : SV_InstanceID;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD;
	
	uint instance_ID : SV_InstanceID;
};
#endif

#endif
