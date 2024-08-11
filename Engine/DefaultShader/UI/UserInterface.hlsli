#ifndef USER_INTERFACE_HLSLI
#define USER_INTERFACE_HLSLI

#ifndef __cplusplus
#include "Engine/DefaultShader/Commons.hlsli"
struct VSIn
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD;
};
#endif

#endif
