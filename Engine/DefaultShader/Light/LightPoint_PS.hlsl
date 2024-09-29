#include "Engine/DefaultShader/Light/Light.hlsli"
#include "Engine/DefaultShader/Light/Light_Func.hlsli"


PS_OUT main(VS_OUT_LightPoint _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	//현재 픽셀의 0.0~1.0 사이의 위치를 찾아낸다.(=UV)
	float2 vUV = _in.position.xy / g_CB_global.fResolution;
	
	//Position 렌더 타겟에는 물체의 view position이 들어가 있다.
	float4 vViewPos = g_view_position_rendertarget.Sample(anisotropicSampler, vUV);
    
	if (0.f == vViewPos.a)
	{
		discard;
	}
	
    // 광원 영역에 잡힌 position target의 위치값을 로컬영역으로 바꿔야한다.
    // 로컬 영역에서 광원메쉬 (sphere)의 내부에 있다면 실제로 point light 안에 들어가있다는 뜻
	float4 vLocalPos = mul(mul(vViewPos, g_CB_camera.inverse_view), g_transforms[_in.instance_ID].InverseWorld);
	
	if (length(vLocalPos.xyz) > 0.5f)
	{
		discard;
	}
    
	float4 vViewNormal = g_normal_rendertarget.Sample(anisotropicSampler, vUV);
	
	tLightColor lightcolor = calculate_light_3D(g_light_attributes[_in.instance_ID], vViewPos.xyz, vViewNormal.xyz);
    
	//float SpecCoef = g_specular_rendertarget.Sample(anisotropicSampler, vUV).x;
	//float4 SpecularColor = DecodeColor(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular = lightcolor.specular; // * SpecularColor.xyz;
	
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
	
	return output;
}
