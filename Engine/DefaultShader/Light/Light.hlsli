#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI
#ifndef __cplusplus//HLSL
#include "../Globals.hlsli"
#include "../Func_DecodeColor.hlsli"

#include "Func_Light.hlsli"


struct VSIn_LightPoint
{
	float4 Position : POSITION;
};

struct VSOut_LightPoint
{
	float4 Position : SV_Position;
};

struct VSIn_LightDir
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOut_LightDir
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
};

struct PS_OUT
{
	float4 vDiffuse : SV_Target0;
	float4 vSpecular : SV_Target1;
};
#endif //HLSL

#endif
