#include "Engine/DefaultShader/Debug/Debug.hlsli"

VSOut main(VSIn _in)
{
	VSOut _out = (VSOut) 0;
	
	_out.Pos = mul(_in.Pos, g_debug[_in.InstanceID].WVP);
	_out.InstanceID = _in.InstanceID;
	
	return _out;
}
