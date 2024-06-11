#include "Engine/Scene/Component/Renderer/Com_Renderer_ParticleSystem.h"

#include "Engine/Scene/Component/Transform/Com_Transform.h"

#include "Engine/Scene/GameObject.h"

#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"
#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Animation/Animation2D.hlsli"



namespace ehw
{
	Com_Renderer_ParticleSystem::Com_Renderer_ParticleSystem()
		: mMaxParticles(100)
		, mStartSize(float4(50.0f, 50.0f, 1.0f, 1.0f))
		, mStartColor(float4(1.0f, 0.2f, 0.2f, 1.0f))
		, mStartLifeTime(3.0f)
		, mFrequency(1.0f)
		, mTime(0.0f)
		, mCBData{}
		, mSimulationSpace(eSimulationSpace::World)
		, mRadius(500.0f)
		, mStartSpeed(200.0f)
		, mElapsedTime(0.0f)

	{

	}

	Com_Renderer_ParticleSystem::Com_Renderer_ParticleSystem(const Com_Renderer_ParticleSystem& _other)
		: Com_Renderer_Mesh(_other)
		, mMaxParticles(_other.mMaxParticles)
		, mStartSize(_other.mStartSize)
		, mStartColor(_other.mStartColor)
		, mStartLifeTime(_other.mStartLifeTime)
		, mFrequency(_other.mFrequency)
		, mTime(_other.mTime)
		, mCBData{ _other.mCBData }
		, mSimulationSpace(_other.mSimulationSpace)
		, mRadius(_other.mRadius)
		, mStartSpeed(_other.mStartSpeed)
		, mElapsedTime(_other.mElapsedTime)
	{
		if (_other.mBuffer != nullptr)
		{
			//mBuffer = _other.mBuffer->Clone();
		}
		if (_other.mSharedBuffer != nullptr)
		{
			//mSharedBuffer = _other.mSharedBuffer->Clone();
		}


	}

	Com_Renderer_ParticleSystem::~Com_Renderer_ParticleSystem()
	{
		delete mBuffer;
		mBuffer = nullptr;

		delete mSharedBuffer;
		mSharedBuffer = nullptr;
	}


	eResult Com_Renderer_ParticleSystem::Serialize_Json(JsonSerializer* Json) const
	{
		return eResult();
	}

	eResult Com_Renderer_ParticleSystem::DeSerialize_Json(const JsonSerializer* Json)
	{
		return eResult();
	}

	void Com_Renderer_ParticleSystem::Init()
	{
		using namespace strKey::defaultRes;
		mCS = LOAD_COMPUTESHADER(ParticleShader);

		std::shared_ptr<Mesh> point = ResourceManager<Mesh>::GetInst().Find(mesh::PointMesh);
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = ResourceManager<Material>::GetInst().Find(material::ParticleMaterial);
		SetMaterial(material, 0);

		std::shared_ptr<Texture> tex = ResourceManager<Texture>::GetInst().Find(texture::CartoonSmoke);
		material->SetTexture(eTextureSlot::Albedo, tex);

		tParticle particles[100] = {};
		float4 startPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = float4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].active = 0;
			particles[i].direction =
				float4(cosf((float)i * (DirectX::XM_2PI / (float)mMaxParticles))
					, sin((float)i * (DirectX::XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f);

			particles[i].speed = 100.0f;
		}

		StructBuffer::Desc sDesc{};
		sDesc.eSBufferType = eStructBufferType::READ_WRITE;

		mBuffer = new StructBuffer(sDesc);
		mBuffer->Create<tParticle>(mMaxParticles, particles, 100u);

		mSharedBuffer = new StructBuffer(sDesc);
		mSharedBuffer->Create<tParticleShared>(1, nullptr, 0u);
	}

	void Com_Renderer_ParticleSystem::FinalUpdate()
	{
		iRenderer::FinalUpdate();

		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;

		//누적시간
		mTime += TimeManager::GetInst().DeltaTime();
		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			uint iAliveCount = (uint)f;
			mTime = f - std::floor(f);

			tParticleShared shared = { 5, };
			mSharedBuffer->SetData(&shared, 1);
		}
		else
		{
			tParticleShared shared = {  };
			mSharedBuffer->SetData(&shared, 1);
		}

		mMaxParticles = mBuffer->GetStride();
		float3 pos = GetOwner()->GetComponent<Com_Transform>()->GetLocalPosition();
		mCBData.worldPosition = float4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.maxParticles = mMaxParticles;
		mCBData.radius = mRadius;
		mCBData.simulationSpace = (uint)mSimulationSpace;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startSize = mStartSize;
		mCBData.startColor = mStartColor;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.deltaTime = TimeManager::GetInst().DeltaTime();
		mCBData.elapsedTime += TimeManager::GetInst().DeltaTime();


		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::ParticleSystem);
		cb->SetData(&mCBData);
		cb->BindData(eShaderStageFlag::ALL);

		mCS->SetSharedStrutedBuffer(mSharedBuffer);
		mCS->SetStrcutedBuffer(mBuffer);
		mCS->OnExcute();
	}

	void Com_Renderer_ParticleSystem::Render()
	{
		if (false == IsRenderReady())
			return;
		
		GetOwner()->GetComponent<Com_Transform>()->BindData();
		mBuffer->BindDataSRV(Register_t_AtlasTexture, eShaderStageFlag::Geometry);

		GetCurrentMaterial(0)->BindData();
		GetMesh()->RenderInstanced(0u, mMaxParticles);

		mBuffer->UnbindData();
	}
}
