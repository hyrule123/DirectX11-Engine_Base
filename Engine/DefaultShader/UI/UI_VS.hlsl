#include "Engine/DefaultShader/UI/UI.hlsli"
VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    Out.Pos = mul(In.Pos, g_transforms[In.instance_ID].WVP);
    Out.UV = In.UV;
    
    return Out;
}
