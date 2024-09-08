#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI
#include "Engine/DefaultShader/Commons.hlsli"
#include "Engine/DefaultShader/Func_DecodeColor.hlsli"

struct alignas(16)    tLightAttribute
{
	tLightColor color;
	float4 position;
	float4 direction;
    
	int lightType;
	float radius;
	float angle;
	int padding;
};

struct alignas(16)  tLightCount
{
	uint count;
	uint3 pad;
};

#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_MAX 2
#define LIGHT_TYPE_SPOT 2

#ifdef __cplusplus
enum class eLightType{
	Directional,
	Point,
	//Spot, //미구현
	END
};
#endif

// Light
SBUFFER(g_light_attributes, tLightAttribute, t, 14);
CBUFFER(g_CB_light_count, tLightCount, b, 5);

struct VS_in_out_LightPoint
{
	float4 Position SEMANTIC(POSITION);
	uint instance_ID SEMANTIC(SV_InstanceID);
};
struct VSIn_LightDir
{
	float4 Position SEMANTIC(POSITION);
	float2 UV SEMANTIC(TEXCOORD);
	uint instance_ID SEMANTIC(SV_InstanceID);
};

#ifdef __HLSL

struct VSOut_LightDir
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
	uint instance_ID : SV_InstanceID;
};

struct PS_OUT
{
	float4 vDiffuse : SV_Target0;
	float4 vSpecular : SV_Target1;
};
#endif //HLSL

#endif
