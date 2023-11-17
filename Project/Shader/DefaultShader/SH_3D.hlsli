#ifndef SH_3D
#define SH_3D
#include "SH_Globals.hlsli"

#ifndef __cplusplus
#include "SH_Func_Light.hlsli"
#include "SH_Func_Animation3D.hlsli"
struct VSIn
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
	
	float3 Tangent : TANGENT;
	float3 BiNormal : BINORMAL;
	float3 Normal : NORMAL;
	
	float4 vWeights : BLENDWEIGHT;
	float4 vIndices : BLENDINDICES;
};

struct VSOut
{
	float4 Position : SV_Position;
	float2 UV : TEXCOORD;
    
	float3 ViewPos : POSITION;
    
	float3 ViewTangent : TANGENT;
	float3 ViewBiNormal : BINORMAL;
	float3 ViewNormal : NORMAL;
	
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
