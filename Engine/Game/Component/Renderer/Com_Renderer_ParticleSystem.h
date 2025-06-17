#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/DefaultShader/Particle/Particle.hlsli"

namespace core
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class ParticleShader;
	class StructBuffer;
	class Com_Renderer_ParticleSystem : public Com_Renderer_Mesh
	{
		CLASS_INFO(Com_Renderer_ParticleSystem, Com_Renderer_Mesh);
		REGISTER_FACTORY(Com_Renderer_ParticleSystem);
		CLONE_ABLE(Com_Renderer_ParticleSystem);
	public:
		Com_Renderer_ParticleSystem();
		Com_Renderer_ParticleSystem(const Com_Renderer_ParticleSystem& _other);

		~Com_Renderer_ParticleSystem();

		virtual eResult serialize_json(JsonSerializer* Json) const override;
		virtual eResult deserialize_json(const JsonSerializer* Json) override;

		virtual void init() override;
		virtual void final_update() override;
		//virtual void render() override;

	private:
#pragma region Buffer 선언
		StructBuffer* m_buffer;
		StructBuffer* m_shared_buffer;
#pragma endregion

#pragma region tParticle GraphicsShader 관련 변수 선언
		s_ptr<ParticleShader> mCS;
		tCB_ParticleSystem mCBData;
#pragma endregion

#pragma region tParticle System 관련 변수 선언
		float4 mStartSize;
		float4 mStartColor;

		eSimulationSpace mSimulationSpace;
		uint mMaxParticles;
		float mStartLifeTime;
		float mFrequency;
		float mRadius;

		float mStartSpeed;
		float mTime;

		//누적시간
		float mElapsedTime;
#pragma endregion
	};

}
