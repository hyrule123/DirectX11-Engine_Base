#include "Engine/DefaultShader/3D/3D.hlsli"

VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0.0f;
	
	tSkinningInfo info = (tSkinningInfo) 0;
	
	//애니메이션 메쉬일 경우 스키닝 진행
	if(TRUE == g_default_3D_mtrl_instancing_buffer[_in.instance_ID].bAnim)
	{
		info = Skinning(
		g_default_3D_mtrl_instancing_buffer[_in.instance_ID].final_bone_matrix_instance_ID
		, g_default_3D_mtrl_instancing_buffer[_in.instance_ID].BoneCount
		, _in.Position.xyz
		, _in.Tangent
		, _in.BiNormal
		, _in.Normal
		, _in.vWeights
		, _in.vIndices
		);
	}
	else
	{
		info.Pos = _in.Position.xyz;
		info.Tangent = _in.Tangent;
		info.Binormal = _in.BiNormal;
		info.Normal = _in.Normal;
	}
    
	_out.Position = mul(float4(info.Pos, 1.f), g_transforms[_in.instance_ID].WVP);
	_out.ViewPos = mul(float4(info.Pos, 1.f), g_transforms[_in.instance_ID].WorldView).xyz;
	
	//뷰 공간에서의 TBN을 구한다. 방향값이므로 동차 좌표는 0
	_out.ViewTangent =	normalize(mul(float4(info.Tangent.xyz, 0.0f), g_transforms[_in.instance_ID].WorldView).xyz);
	_out.ViewBiNormal =	normalize(mul(float4(info.Binormal.xyz, 0.0f), g_transforms[_in.instance_ID].WorldView).xyz);
	_out.ViewNormal =	normalize(mul(float4(info.Normal.xyz, 0.0f), g_transforms[_in.instance_ID].WorldView).xyz);
	
	_out.UV = _in.UV;
    
	return _out;
}

