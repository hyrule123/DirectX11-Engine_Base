#include "Engine/DefaultShader/Light/Light.hlsli"


VS_in_out_LightPoint main(VS_in_out_LightPoint _in)
{
	_in.Position.w = 1.f;
	_in.Position = mul(float4(_in.Position), g_transforms[_in.instance_ID].WVP);
	return _in;
}
