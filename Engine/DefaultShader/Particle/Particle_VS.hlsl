#include "Engine/DefaultShader/Particle/Particle.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.0f;
    
    Out.Pos = In.Pos;
    Out.instance_ID = In.instance_ID;
    
    return Out;
}
