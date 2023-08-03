#pragma once
#include "IRenderer.h"
#include "ParticleShader.h"

namespace mh
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class StructBuffer;

	class Com_Renderer_ParticleSystem : public IRenderer
	{
	public:
		Com_Renderer_ParticleSystem();
		~Com_Renderer_ParticleSystem();

		virtual void Init() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
#pragma region Buffer 선언
		StructBuffer* mBuffer;
		StructBuffer* mSharedBuffer;
#pragma endregion
#pragma region tParticle GraphicsShader 관련 변수 선언
		std::shared_ptr<ParticleShader> mCS;
		ParticleSystemCB mCBData;
#pragma endregion
#pragma region tParticle System 관련 변수 선언
		Vector4 mStartSize;
		Vector4 mStartColor;
		
		eSimulationSpace mSimulationSpace;
		UINT mMaxParticles;
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