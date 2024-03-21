#include "Engine/DefaultShader/Basic/_3D.hlsli"

PSOut_Deffered main(VSOut _in)
{
	float4 vObjectColor = float4(0.4f, 0.4f, 0.4f, 1.f);
	float4 vOutColor = float4(1.f, 1.f, 1.f, 1.f);
	float3 vViewNormal = _in.ViewNormal;
    
	if (TRUE == CB_MaterialData.IsAlbedoTex)
	{
		vObjectColor = AlbedoTexture.Sample(anisotropicSampler, _in.UV);
	}
    
	if (TRUE == CB_MaterialData.IsNormalTex)
	{
		float3x3 matRot =
		{
			_in.ViewTangent,//X
			_in.ViewBiNormal,//Y
			_in.ViewNormal//Z
		};

		// 물체의 표면에 적용될 탄젠트 공간 기준 방향벡터를 가져온다.
		float3 vNormal = NormalTexture.Sample(anisotropicSampler, _in.UV).xyz;
		
		//Normal 값은 0.f과 1.f 사이(0 ~ 255) -> -1 ~ 1 사이로 변환한다.
		// 0~1값을 -1~1의 값으로 변환 후 
		vNormal = normalize((vNormal * 2.f) - 1.f);
		
		//Normal 회전 행렬과 곱해서 실제 노말값을 구한다.
		vViewNormal = mul(vNormal, matRot);
	}
	
	float4 SpecularCoeff = CB_MaterialData.Spec;
	if (TRUE == CB_MaterialData.IsSpecularTex)
	{
		SpecularCoeff = SpecularTexture.Sample(anisotropicSampler, _in.UV);
	}
    
	
	PSOut_Deffered OutColor = (PSOut_Deffered) 0;
	
	OutColor.Albedo = vObjectColor;
	OutColor.Normal = float4(vViewNormal.xyz, 1.f);
	OutColor.Specular = SpecularCoeff;
	OutColor.Emissive = float4(1.0f, 1.0f, 1.0f, 1.0f);
	OutColor.Position = float4(_in.ViewPos.xyz, 1.f);
	
	return OutColor;
}
