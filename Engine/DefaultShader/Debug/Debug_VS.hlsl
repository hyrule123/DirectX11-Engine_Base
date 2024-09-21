#include "Engine/DefaultShader/Debug/Debug.hlsli"

VS_INOUT main(VS_INOUT _in)
{
	_in.pos = mul(_in.pos, g_debugDrawData[_in.instance_ID].WVP);
	
	return _in;
}
