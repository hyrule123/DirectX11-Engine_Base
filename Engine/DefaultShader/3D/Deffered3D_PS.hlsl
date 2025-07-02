#include <Engine/DefaultShader/3D/3D.hlsli>

PSOut_Deffered main(VSOut _in)
{
	PSOut_Deffered out_color = (PSOut_Deffered) 0;
	
	float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
	float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
	float3 vViewNormal = _in.ViewNormal;
    
	if (TRUE == g_CB_material_shared_data.IsAlbedoTex)
	{
		vObjectColor = g_diffuse_texture.Sample(anisotropicSampler, _in.UV);
	}
    
	if (TRUE == g_CB_material_shared_data.IsNormalTex)
	{
		//normal 회전 행렬(view 공간으로 normal값을 변환해주는 행렬)
		float3x3 matRot =
		{
			_in.ViewTangent,//X
			_in.ViewBiNormal,//Y
			_in.ViewNormal//Z
		};

		//노말맵에 들어있는 방향 벡터는 '탄젠트 공간'에 속해 있는 방향 벡터이다.
		float3 vNormal = g_normal_texture.Sample(anisotropicSampler, _in.UV).xyz;
		
		//Normal 값은 0.f과 1.f 사이(0 ~ 255) -> -1 ~ 1 사이로 변환한다.
		// 0~1값을 -1~1의 값으로 변환한다.
		vNormal = normalize((vNormal * 2.f) - 1.f);
		
		//Normal 회전 행렬과 곱해서 실제 노말값을 구한다.
		vViewNormal = mul(vNormal, matRot);
	}
	
	float4 SpecularCoeff = g_CB_material_shared_data.specular;
	if (TRUE == g_CB_material_shared_data.IsSpecularTex)
	{
		SpecularCoeff = g_specular_texture.Sample(anisotropicSampler, _in.UV);
	}
    
	
	PSOut_Deffered deffered_out = (PSOut_Deffered) 0;
	
	deffered_out.Diffuse = vObjectColor;
	deffered_out.Normal = float4(vViewNormal.xyz, 1.f);
	deffered_out.Specular = SpecularCoeff;
	deffered_out.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
	deffered_out.Position = float4(_in.ViewPos.xyz, 1.f);
	
	return deffered_out;
}
