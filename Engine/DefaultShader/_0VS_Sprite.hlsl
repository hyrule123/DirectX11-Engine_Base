#include "SH_Animation2D.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
	Out.WorldPos = mul(In.Pos, CB_Transform.World);
    Out.Pos = mul(In.Pos, CB_Transform.WVP);
    Out.UV = In.UV;
    
    return Out;
}
