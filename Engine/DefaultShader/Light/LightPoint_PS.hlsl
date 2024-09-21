#include "Engine/DefaultShader/Light/Light.hlsli"
#include "Engine/DefaultShader/Light/Light_Func.hlsli"


PS_OUT main(VS_in_out_LightPoint _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	float2 vUV = _in.Position.xy / CB_Global.fResolution;
	
	//Position 렌더 타겟에는 View Position 값이 들어가 있다.
	float4 vViewPos = PositionTarget.Sample(anisotropicSampler, vUV);
    
	if (0.f == vViewPos.a)
	{
		discard;
	}
      
    // 광원 영역에 잡힌 position target의 위치값을 로컬영역으로 바꿔야한다.
    // 로컬 영역에서 광원메쉬 (sphere)의 내부에 있다면 실제로 point light 안에 들어가있다는 뜻
	float4 vLocalPos = mul(mul(vViewPos, g_CB_camera.inverse_view), g_transforms[_in.instance_ID].InverseWorld);
	
	if (length(vLocalPos.xyz) > g_light_attributes[_in.instance_ID].radius)
	{
		discard;
	}
    
	float4 vViewNormal = NormalTarget.Sample(anisotropicSampler, vUV);
	
	tLightColor lightcolor = calculate_light_3D(_in.instance_ID, vViewPos.xyz, vViewNormal.xyz);
    
	float SpecCoef = SpecularTarget.Sample(anisotropicSampler, vUV).x;
	float4 SpecularColor = DecodeColor(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular.xyz = lightcolor.specular.xyz; // * SpecularColor.xyz;
	
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
	
	return output;
}