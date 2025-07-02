#include <Engine/DefaultShader/Particle/Particle.hlsli>



[maxvertexcount(6)]
void main( point VSOut input[1], inout TriangleStream<GSOutput> output)
{
    GSOutput Out[4] = { (GSOutput) 0.0f, (GSOutput) 0.0f, (GSOutput) 0.0f, (GSOutput) 0.0f };
	
    if (0 == ParticleBuffer[input[0].instance_ID].active)
        return;
	
	float3 vWorldPos = input[0].Pos.xyz + ParticleBuffer[input[0].instance_ID].position.xyz;
    
    if (g_CB_particle_system.simulationSpace == 0)
    {
		vWorldPos += g_transforms[input[0].instance_ID].World._41_42_43;
	}
	float3 vViewPos = mul(float4(vWorldPos, 1.0f), g_CB_camera.view).xyz;
	
    float3 Scale = g_CB_particle_system.startSize.xyz;
    //Scale = lerp(20.0f, 50.0f, elapsedTime);
	
    float3 NewPos[4] =
    {
        vViewPos + float3(-0.5f, 0.5f, 0.0f) *  Scale,
        vViewPos + float3(0.5f, 0.5f, 0.0f)  *   Scale,
        vViewPos + float3(0.5f, -0.5f, 0.0f) *  Scale,
        vViewPos + float3(-0.5f, -0.5f, 0.0f) * Scale
    };
	
    for (int i = 0; i < 4; i++)
    {
		Out[i].Pos = mul(float4(NewPos[i], 1.0f), g_CB_camera.projection);
	}
	
    Out[0].UV = float2(0.0f, 0.0f);
    Out[1].UV = float2(1.0f, 0.0f);
    Out[2].UV = float2(1.0f, 1.0f);
    Out[3].UV = float2(0.0f, 1.0f);
	
    Out[0].instance_ID = input[0].instance_ID;
    Out[1].instance_ID = input[0].instance_ID;
    Out[2].instance_ID = input[0].instance_ID;
    Out[3].instance_ID = input[0].instance_ID;
    
    
    // 0 -- 1
    // | \  |
    // 3 -- 2
    
    output.Append(Out[0]);
    output.Append(Out[1]);
    output.Append(Out[2]);
    output.RestartStrip();
    
    output.Append(Out[0]);
    output.Append(Out[2]);
    output.Append(Out[3]);
    output.RestartStrip();
}
