#pragma once
#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/DefaultShader/Particle/Particle.hlsli"

namespace ehw
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
	public:
		Com_Renderer_ParticleSystem();
		Com_Renderer_ParticleSystem(const Com_Renderer_ParticleSystem& _other);
		CLONE_ABLE(Com_Renderer_ParticleSystem);

		~Com_Renderer_ParticleSystem();

		virtual eResult Serialize_Json(JsonSerializer* Json) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* Json) override;

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() override;

	private:
#pragma region Buffer 선언
		StructBuffer* mBuffer;
		StructBuffer* mSharedBuffer;
#pragma endregion

#pragma region tParticle GraphicsShader 관련 변수 선언
		std::shared_ptr<ParticleShader> mCS;
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
