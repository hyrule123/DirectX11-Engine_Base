#ifndef ANIMATION_2D_HLSLI
#define ANIMATION_2D_HLSLI

#include "Common.hlsli"
//Atlas texture(Animation 2D)
TEXTURE2D(AtlasTexture, t, 0);
#define IsAtlasTex bTex_0

#ifndef __cplusplus
#include "Func_Light.hlsli"
struct VSIn
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float3 WorldPos : POSITION;
	float2 UV : TEXCOORD;
};
#endif//__cplusplus

#endif
