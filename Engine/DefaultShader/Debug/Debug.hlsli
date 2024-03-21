#ifndef DEBUG_HLSLI
#define DEBUG_HLSLI
#include "Engine/DefaultShader/Common.hlsli"

struct alignas(16)  tDebug
{
	MATRIX WVP;
	BOOL isColliding;
	float3 pad0;
};
SBUFFER(g_debug, tDebug, t, 126);

#ifdef __HLSL
struct VSIn
{
	float4 Pos : POSITION;
	uint InstanceID : SV_InstanceID;
};

struct VSOut
{
	float4 Pos : SV_Position;
	uint InstanceID : SV_InstanceID;
};
#endif //__HLSL

#endif //DEBUG_HLSLI
