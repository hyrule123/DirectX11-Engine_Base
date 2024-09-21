#ifndef DEBUG_HLSLI
#define DEBUG_HLSLI
#include "Engine/DefaultShader/Common_define.hlsli"

struct alignas(16)  tDebugDrawData
{
	MATRIX WVP;
	float3 RGB;
	float pad;
};
SBUFFER(g_debugDrawData, tDebugDrawData, t, 126);

#ifdef __HLSL
struct VS_INOUT
{
	float4 pos : POSITION;
	uint instance_ID : SV_InstanceID;
};
#endif //__HLSL

#endif //DEBUG_HLSLI
