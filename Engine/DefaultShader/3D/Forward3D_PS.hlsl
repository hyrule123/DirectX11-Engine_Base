#include <Engine/DefaultShader/3D/3D.hlsli>

float4 main(VSOut In) : SV_Target
{
	float4 OutColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	if (g_CB_material_shared_data.IsAlbedoTex == TRUE)
	{
		OutColor = g_diffuse_texture.Sample(anisotropicSampler, In.UV);
	}

	float3 vNormal = In.ViewNormal;

	if (g_CB_material_shared_data.IsNormalTex == TRUE)
	{
	// 물체의 표면에 적용될 탄젠트 공간 기준 방향벡터를 가져온다.
		vNormal = g_normal_texture.Sample(anisotropicSampler, In.UV).xyz;

	// 0~1값을 -1~1의 값으로 변환
		vNormal = (vNormal * 2.0f) - 1.0f;

		float3x3 matTBN =
		{
			In.ViewTangent,
			In.ViewBiNormal,
			In.ViewNormal,
		};

		vNormal = normalize(mul(vNormal, matTBN));
	}
	
	tLightColor lightColor = (tLightColor)0.0f;
	for (uint i = 0; i < g_CB_light_count.count; i++)
	{
		tLightColor temp = calculate_light_3D(g_light_attributes[i], In.ViewPos, vNormal);
		lightColor.ambient += temp.ambient;
		lightColor.diffuse += temp.diffuse;
		lightColor.specular += temp.specular;
	}

	OutColor.rgb = (OutColor.rgb * lightColor.diffuse.rgb
					+ lightColor.specular.rgb
					+ (OutColor.xyz * lightColor.ambient.rgb));


	return OutColor;
        
}
