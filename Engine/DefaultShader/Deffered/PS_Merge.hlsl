#include "../Basic/_3D.hlsli"

float4 main(VSOut_Merge In) : SV_Target
{
	float4 OutColor = (float4) 0.0f;
	float2 UV = In.Position.xy / CB_Global.fResolution;
    
	float4 viewPos = PositionTarget.Sample(anisotropicSampler, UV);
	if (0.0f == viewPos.a)
		discard;
    
	OutColor = AlbedoTarget.Sample(anisotropicSampler, UV);
    
	float4 vDiffuse = DiffuseLightTarget.Sample(anisotropicSampler, UV);
	float4 vSpecular = SpecularLightTarget.Sample(anisotropicSampler, UV);
    
	OutColor.rgb = (OutColor.rgb * vDiffuse.rgb) + vSpecular.rgb;
    
    
	return OutColor;
}
