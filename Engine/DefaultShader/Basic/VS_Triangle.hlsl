#include "Engine/DefaultShader/Commons.hlsli"
struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
    
	uint instance_ID : SV_InstanceID;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float2 UV : TEXCOORD;
    
	uint instance_ID : SV_InstanceID;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
	Out.Pos = mul(In.Pos, g_transforms[In.instance_ID].WVP);
	Out.UV = In.UV;
	Out.instance_ID = In.instance_ID;
    
    return Out;
}
