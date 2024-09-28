#include "Engine/DefaultShader/UI/UI.hlsli"


float4 main(VSOut In) : SV_Target
{
    float4 color = (float) 0.0f;
    color = g_is_texture_0.Sample(anisotropicSampler, In.UV);
    
    return color;
}
