#include "Engine/DefaultShader/3D/3D.hlsli"

float4 main(VSOut_Merge In) : SV_Target
{
	float4 OutColor = (float4) 0.0f;
	float2 UV = In.Position.xy / CB_Global.fResolution;
    
	float4 viewPos = g_view_position_rendertarget.Sample(anisotropicSampler, UV);
	if (0.0f == viewPos.a)
	{
		discard;
	}
    
	OutColor = g_diffuse_rendertarget.Sample(anisotropicSampler, UV);
    
	float4 vDiffuse = g_diffuse_light_rendertarget.Sample(anisotropicSampler, UV);
	float4 vSpecular = g_specular_light_rendertarget.Sample(anisotropicSampler, UV);
    
	OutColor.rgb = (OutColor.rgb * vDiffuse.rgb) + vSpecular.rgb;
    
    
	return OutColor;
}
