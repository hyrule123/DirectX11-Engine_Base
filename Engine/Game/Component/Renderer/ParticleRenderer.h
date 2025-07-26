#pragma once
#include <Engine/Game/Component/Renderer/StaticMeshRenderer.h>

#include <Engine/DefaultShader/Particle/Particle.hlsli>

namespace core
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class ParticleShader;
	class StructBuffer;
	class ParticleRenderer : public StaticMeshRenderer
	{
		CLASS_INFO(ParticleRenderer, StaticMeshRenderer);
		REGISTER_FACTORY(ParticleRenderer);
		CLONE_ABLE(ParticleRenderer);

	public:
		ParticleRenderer();
		ParticleRenderer(const ParticleRenderer& _other);

		~ParticleRenderer();

		virtual void init() override;
		virtual void final_update() override;
		//virtual void render() override;

	private:
#pragma region Buffer 선언
		s_ptr<StructBuffer> m_buffer;
		s_ptr<StructBuffer> m_shared_buffer;
#pragma endregion

#pragma region tParticle GraphicsShader 관련 변수 선언
		s_ptr<ParticleShader> m_particle_CS;
		tCB_ParticleSystem mCBData;
#pragma endregion

#pragma region tParticle System 관련 변수 선언
		float4 m_start_size;
		float4 m_start_color;

		eSimulationSpace m_simulation_space;
		uint m_max_particles;
		float m_start_lifetime;
		float m_frequency;
		float m_radius;

		float m_start_speed;
		float m_time;

		//누적시간
		float m_elapsed_time;
#pragma endregion
	};

}
