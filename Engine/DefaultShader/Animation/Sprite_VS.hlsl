#include "Animation2D.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
	Out.WorldPos = mul(In.Pos, g_transforms[In.instance_ID].World).xyz;
	Out.Pos = mul(In.Pos, g_transforms[In.instance_ID].WVP);
    Out.UV = In.UV;
	Out.instance_ID = In.instance_ID;
    
    return Out;
}
