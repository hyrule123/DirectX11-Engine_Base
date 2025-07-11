#include <Engine/DefaultShader/Particle/Particle.hlsli>
#include <Engine/DefaultShader/Func_Blur.hlsli>

//각각의 스레드가 동기화할 데이터가 필요하다

//1024개가 최대 스레드 개수
[numthreads(128, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    if ( g_CB_particle_system.maxParticles <= DTid.x )
        return;
    
    if (RW_ParticleBuffer[DTid.x].active == 0)
    {
        while (0 < RW_SharedParticleBuffer[0].gActiveCount)
        {
			int originValue = RW_SharedParticleBuffer[0].gActiveCount;
            int exchange = originValue - 1;
            
            // 스레드 동기화
            // dest값을 exchange값으로 바꾸는 동안
            // 다른스레드는 멈춘다.
            //InterlockedExchange(ParticleSharedBuffer[0].gActiveCount, exchange, exchange);
            InterlockedCompareExchange(RW_SharedParticleBuffer[0].gActiveCount
                                        , originValue, exchange, exchange);
            
            if (originValue == exchange)
            {
                RW_ParticleBuffer[DTid.x].active = 1;
                break;
            }
        }
        
		if (RW_ParticleBuffer[DTid.x].active)
        {
                // 랜덤값으로 위치와 방향을 설정해준다.
                // 샘플링을 시도할 UV 계산해준다.
            float4 Random = (float4) 0.0f;
            float2 UV = float2((float) DTid.x / g_CB_particle_system.maxParticles, 0.5f);
            UV.x += g_CB_particle_system.elapsedTime;
			UV.y += sin((UV.x + g_CB_particle_system.elapsedTime) * 3.14592f + 2.0f * 10.0f) * 0.5f;
                
            Random = float4
                (
                    GaussianBlur(UV + float2(0.0f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.1f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.2f, 0.0f)).x
                    , GaussianBlur(UV + float2(0.2f, 0.0f)).x
                );

            //// radius 원형 범위로 스폰
            float fTheta = Random.x * 3.141592f * 2.0f;
			RW_ParticleBuffer[DTid.x].position.xy = float2(cos(fTheta), sin(fTheta)) * Random.y * g_CB_particle_system.radius;
            RW_ParticleBuffer[DTid.x].position.z = 100.0f;
            
            RW_ParticleBuffer[DTid.x].direction.xy 
                = normalize(float2(RW_ParticleBuffer[DTid.x].position.xy));
            
            if (g_CB_particle_system.simulationSpace) // 1 world , 0 local
            {
                RW_ParticleBuffer[DTid.x].position.xyz += g_CB_particle_system.worldPosition.xyz;
            }
            
            ////파티클 속력
            RW_ParticleBuffer[DTid.x].time = 0.0f;
			RW_ParticleBuffer[DTid.x].speed = g_CB_particle_system.startSpeed;
			RW_ParticleBuffer[DTid.x].lifeTime = g_CB_particle_system.startLifeTime;
            
        }
    }
    else // active == 1
    {
		RW_ParticleBuffer[DTid.x].time += g_CB_particle_system.deltaTime;
        if (RW_ParticleBuffer[DTid.x].lifeTime < RW_ParticleBuffer[DTid.x].time)
        {
            RW_ParticleBuffer[DTid.x].active = 0;
        }
        else
        {
            RW_ParticleBuffer[DTid.x].position 
            += RW_ParticleBuffer[DTid.x].direction * RW_ParticleBuffer[DTid.x].speed * g_CB_particle_system.deltaTime;
        }
    }
}
