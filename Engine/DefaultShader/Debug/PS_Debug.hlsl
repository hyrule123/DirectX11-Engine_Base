#include "Engine/DefaultShader/Debug/Debug.hlsli"

float4 main(VSOut _in) : SV_TARGET
{
	if (TRUE == g_debugDrawData[_in.InstanceID].isColliding)
	{
		return float4(1.f, 0.f, 0.f, 1.f);
	}

	return float4(0.f, 1.f, 0.f, 1.f);
}
