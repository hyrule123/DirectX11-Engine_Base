#include "Engine/DefaultShader/Light/Light.hlsli"


VS_OUT_LightPoint main(VS_IN_LightPoint _in)
{
	VS_OUT_LightPoint _out = (VS_OUT_LightPoint) 0;
	
	_out.position = mul(float4(_in.position), g_transforms[_in.instance_ID].WVP);
	_out.instance_ID = _in.instance_ID;
	
	return _out;
}
