#include "Engine/DefaultShader/Debug/Debug.hlsli"

VS_OUT main(VS_IN _in)
{
	VS_OUT _out = (VS_OUT) 0;
	_out.pos = mul(_in.pos, g_debugDrawData[_in.instance_ID].WVP);
	_out.instance_ID = _in.instance_ID;
	//_out.pos = float4(0.f, 0.f, 0.f, 1.f);
	return _out;
}
