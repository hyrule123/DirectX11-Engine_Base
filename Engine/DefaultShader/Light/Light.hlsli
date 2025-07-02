#ifndef LIGHT_HLSLI
#define LIGHT_HLSLI
#include <Engine/DefaultShader/Commons.hlsli>
#include <Engine/DefaultShader/Func_DecodeColor.hlsli>

struct alignas(16)    tLightAttribute
{
	tLightColor color;
	float4 position;
	float4 direction;//Directional Light에서 사용
	int lightType;	
	float radius;	//Transform의 Scale을 사용하지 않는 이유: Mesh의 크기와 별도로 사이즈를 조절할 수 있도록
	float angle;	//Spot Light에서 사용
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

	namespace core{
	enum class eLightType{
		Directional,
		Point,
		//Spot, //미구현
		END
	};

	enum class eMRT_Light
	{
		Diffuse_Light,
		Specular_Light,
		END
	};
	namespace name
	{
		STR_KEY eMRT_Light_String[(int)eMRT_Light::END]
		{
			"MRT_diffuse_light",
			"MRT_specular_light",
		};
	}
}
#endif



// Light
SBUFFER(g_light_attributes, tLightAttribute, t, 14);
CBUFFER(g_CB_light_count, tLightCount, b, 5);

#ifdef __HLSL

struct VS_IN_LightPoint
{
	float4 position : POSITION;
	uint instance_ID : SV_InstanceID;
};
struct VS_OUT_LightPoint
{
	float4 position : SV_Position;
	uint instance_ID : SV_InstanceID;
};

struct VSIn_LightDir
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	uint instance_ID : SV_InstanceID;
};

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
