#ifndef DEBUG_HLSLI
#define DEBUG_HLSLI
#include "Engine/DefaultShader/Common.hlsli"

struct alignas(16)  tDebug
{
	MATRIX WVP;
	BOOL isColliding;
	float3 pad0;
};
SBUFFER(g_Debug, tDebug, t, 126);

#ifdef __HLSL
struct VS_IN
{
	
};
#endif //__HLSL

#endif //DEBUG_HLSLI
