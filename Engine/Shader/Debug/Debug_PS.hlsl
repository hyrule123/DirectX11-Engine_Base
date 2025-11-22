#include <Engine/Shader/Debug/Debug.hlsli>

float4 main(VS_OUT _in) : SV_TARGET
{
	return float4(g_debugDrawData[_in.instance_ID].RGB, 1.f);
}
