#include <Engine/DefaultShader/Particle/Particle.hlsli>

float4 main(GSOutput In) : SV_TARGET
{
    float4 outColor = (float4) 0.0f;
    
    outColor = g_diffuse_texture.Sample(anisotropicSampler, In.UV);
    
    outColor *= g_CB_particle_system.startColor;
    
    if (outColor.a <= 0.0f)
        discard;
    
    //if (0.0f < NoiseTime)
    //{
    //    outColor.a -= NoiseTexture.Sample(anisotropicSampler, In.UV).x * 0.25f * NoiseTime;
    //}
    
    return outColor;
}
