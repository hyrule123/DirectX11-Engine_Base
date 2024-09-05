#include "Engine/DefaultShader/Light/Light.hlsli"

		//albedo = Resources::Find<Texture>(L"PositionTarget");
		//lightMaterial->SetTexture(eTextureSlot::PositionTarget, albedo);
		//albedo = Resources::Find<Texture>(L"NormalTarget");
		//lightMaterial->SetTexture(eTextureSlot::NormalTarget, albedo);
		//albedo = Resources::Find<Texture>(L"SpecularTarget");
		//lightMaterial->SetTexture(eTextureSlot::SpecularTarget, albedo);

PS_OUT main(VSOut_LightPoint _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	float2 vUV = _in.Position.xy / CB_Global.fResolution;
	float4 vViewPos = PositionTarget.Sample(anisotropicSampler, vUV);
    
	if (0.f == vViewPos.a)
	{
		discard;
	}
      
    // 광원 영역에 잡힌 position target의 위치값을 로컬영역으로 바꿔야한다.
    // 로컬 영역에서 광원메쉬 (sphere)의 내부에 있다면 실제로 point light 안에 들어가있다는 뜻
	float4 vLocalPos = mul(mul(vViewPos, CB_Transform.InverseView), CB_Transform.InverseWorld);
	

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			
		}
	}
	
	
	//로컬 공간의 구체는 반지름이 0.5f이다.(이 거리 바깥에 있는 픽셀의 경우 빛의 거리 바깥에 있는것
	if (length(vLocalPos.xyz) == 1.f)
	{
		{
			output.vDiffuse = float4(1.f, 1.f, 1.f, 1.f);
			output.vSpecular = float4(1.f, 1.f, 1.f, 1.f);
			return output;
		}
		discard;
	}
    
	float4 vViewNormal = NormalTarget.Sample(anisotropicSampler, vUV);
	
	tLightColor lightcolor = (tLightColor) 0.f;
	CalculateLight3D(vViewPos.xyz, vViewNormal.xyz, CB_NumberOfLight.indexOfLight, lightcolor);
    
	float SpecCoef = SpecularTarget.Sample(anisotropicSampler, vUV).x;
	float4 SpecularColor = DecodeColor(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular.xyz = lightcolor.specular.xyz; // * SpecularColor.xyz;

	
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
    
	
	return output;
}
