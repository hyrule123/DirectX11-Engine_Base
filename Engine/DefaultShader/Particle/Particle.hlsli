#ifndef PARTICLE_HLSLI
#define PARTICLE_HLSLI
#include <Engine/DefaultShader/Commons.hlsli>


struct alignas(16)   tParticle
{
	float4 position;
	float4 direction;
    
	float lifeTime;
	float time;
	float speed;
	uint active;
};

struct alignas(16)    tParticleShared
{
	uint gActiveCount;
};

// Particle
//StructuredBuffer<Particle> particleBuffer : register(t15);
//RWStructuredBuffer<Particle> ParticleBuffer : register(u0);
//RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);
SBUFFER(ParticleBuffer, tParticle, t, 15);
SBUFFER_RW(RW_ParticleBuffer, tParticle, u, 1);
SBUFFER_RW(RW_SharedParticleBuffer, tParticleShared, u, 2);

//HLSL
#ifndef __cplusplus
struct VSIn
{
	float4 Pos : POSITION;
	uint instance_ID : SV_InstanceID;
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
	uint instance_ID : SV_InstanceID;
};

struct VSOut
{
	float4 Pos : SV_Position;
	uint instance_ID : SV_InstanceID;
};
#endif//HLSL
#endif
