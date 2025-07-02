#ifndef SAMPLER_HLSLI
#define SAMPLER_HLSLI
#include <Engine/DefaultShader/Common_define.hlsli>

SAMPLER(pointSampler, s, 0);
SAMPLER(linearSampler, s, 1);
SAMPLER(anisotropicSampler, s, 2);

#endif
