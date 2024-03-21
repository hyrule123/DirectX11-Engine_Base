#include "Engine/DefaultShader/Light/Light.hlsli"


VSOut_LightPoint main(VSIn_LightPoint In)
{
	VSOut_LightPoint Out = (VSOut_LightPoint) 0.0f;
    
	Out.Position = mul(float4(In.Position), CB_Transform.WVP);
	//Out.Position.w = 1.f;
    
	return Out;
}
