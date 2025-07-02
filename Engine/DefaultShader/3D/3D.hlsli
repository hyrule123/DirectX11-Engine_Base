#ifndef _3D_HLSLI
#define _3D_HLSLI
#include <Engine/DefaultShader/Commons.hlsli>

#ifndef __cplusplus
#include <Engine/DefaultShader/Light/Light.hlsli>
#include <Engine/DefaultShader/Light/Light_Func.hlsli>
#include <Engine/DefaultShader/Animation/Animation3D_Func.hlsli>



struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float3 Normal : NORMAL;
	
	uint4 vIndices : BLENDINDICES;
	float4 vWeights : BLENDWEIGHT;
	
	uint instance_ID : SV_InstanceID;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
    
	float3 ViewPos : POSITION;
    
	float3 ViewTangent : TANGENT;
	float3 ViewBiNormal : BINORMAL;
	float3 ViewNormal : NORMAL;
	
	uint instance_ID : SV_InstanceID;
	
};

//수정 시 아래 C++ enum도 수정할것.
struct PSOut_Deffered
{
	float4 Diffuse : SV_Target0;
	float4 Specular : SV_Target1;
	float4 Normal : SV_Target2;
	float4 Position : SV_Target3;
	float4 Emissive : SV_Target4;
	float4 RoughnessAndMetalic : SV_Target5;
};
#endif
#ifdef __cplusplus
namespace core{
	enum class eMRT_Deffered
	{
		Diffuse,
		Specular,
		Normal,
		ViewPosition,
		Emissive,
		Roughness_Metalic,
		END
	};
	namespace name
	{
		STR_KEY eMRT_Deffered_String[(int)eMRT_Deffered::END]
		{
			"RT_diffuse",
			"RT_specular",
			"RT_normal",
			"RT_view_position",
			"RT_emissive",
			"RT_roughness_metalic",	
		};
	}
}
#endif //__cplusplus

#ifdef __HLSL
struct VSIn_Merge
{
	float4 Position : POSITION;
};

struct VSOut_Merge
{
	float4 Position : SV_Position;
};
#endif//HLSL

#endif//_3D_HLSLI
