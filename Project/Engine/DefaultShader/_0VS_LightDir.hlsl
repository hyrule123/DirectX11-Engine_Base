#include "SH_Light.hlsli"


//직사광선은 화면 전체를 덮으므로 
VSOut_LightDir main(VSIn_LightDir In)
{
	VSOut_LightDir Out = (VSOut_LightDir) 0.0f;
    
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
	Out.UV = In.UV;
    
	return Out;
}
