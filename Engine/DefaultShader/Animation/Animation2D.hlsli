#ifndef ANIMATION_2D_HLSLI
#define ANIMATION_2D_HLSLI

#include "Engine/DefaultShader/Common_define.hlsli"


//Atlas texture(Animation 2D)
TEXTURE2D(AtlasTexture, t, 0);
#define IsAtlasTex bTex_0

#ifdef __HLSL
#include "Engine/DefaultShader/Light/Light_Func.hlsli"

struct VSIn
{
	float4 Pos : POSITION;
	float2 UV : TEXCOORD;
	uint instance_ID : SV_InstanceID;
};

struct VSOut
{
	float4 Pos : SV_Position;
	float3 WorldPos : POSITION;
	float2 UV : TEXCOORD;
	uint instance_ID : SV_InstanceID;
};
#endif//HLSL

#endif
