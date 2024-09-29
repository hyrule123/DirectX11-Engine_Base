#include "Engine/DefaultShader/Light/Light.hlsli"
#include "Engine/DefaultShader/Light/Light_Func.hlsli"


PS_OUT main(VSOut_LightDir _in)
{
	PS_OUT output = (PS_OUT) 0.f;
    
	float2 vUV = _in.Position.xy / g_CB_global.fResolution;
	float4 vViewPos = g_view_position_rendertarget.Sample(anisotropicSampler, vUV);
    
	//xyz 셋다 0일 경우 버림
	if (false == any(vViewPos.xyz))
	{
		discard;
	}
      
	float4 vViewNormal = g_normal_rendertarget.Sample(anisotropicSampler, vUV);
	
	tLightColor lightcolor = calculate_light_3D(g_light_attributes[_in.instance_ID], vViewPos.xyz, vViewNormal.xyz);
	
	//tLightColor lightcolor = calculate_directional_light_3D(g_light_attributes[_in.instance_ID], vViewPos.xyz, vViewNormal.xyz);
    
	float SpecCoef = g_specular_rendertarget.Sample(anisotropicSampler, vUV).x;
	float4 SpecularColor = DecodeColor(SpecCoef);

	output.vDiffuse = lightcolor.diffuse + lightcolor.ambient;
	output.vSpecular = lightcolor.specular; // * SpecularColor.xyz;
       
	output.vDiffuse.a = 1.f;
	output.vSpecular.a = 1.f;
    
	return output;
}
