#include "Engine/DefaultShader/3D/3D.hlsli"

VSOut_Merge main(VSIn_Merge In)
{
	VSOut_Merge Out = (VSIn_Merge) 0.0f;
    
	Out.Position = float4(In.Position.xy * 2.0f, 0.0f, 1.0f);
    
	return Out;
}
