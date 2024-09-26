#ifndef _3D_HLSLI
#define _3D_HLSLI
#include "Engine/DefaultShader/Commons.hlsli"



#ifndef __cplusplus
#include "Engine/DefaultShader/Light/Light.hlsli"
#include "Engine/DefaultShader/Light/Light_Func.hlsli"
#include "Engine/DefaultShader/Animation/Animation3D_Func.hlsli"

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

struct PSOut_Deffered
{
	float4 Albedo : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Specular : SV_Target2;
	float4 Emissive : SV_Target3;
	float4 RoughnessAndMetalic : SV_Target4;
	float4 Position : SV_Target5;
};

struct VSIn_Merge
{
	float4 Position : POSITION;
};

struct VSOut_Merge
{
	float4 Position : SV_Position;
};
#endif//__cplusplus

#endif
