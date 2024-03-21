#ifndef SAMPLER_HLSLI
#define SAMPLER_HLSLI
#include "Engine/DefaultShader/Common.hlsli"

SAMPLER(pointSampler, s, 0);
SAMPLER(linearSampler, s, 1);
SAMPLER(anisotropicSampler, s, 2);

#endif
