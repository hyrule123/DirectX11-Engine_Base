
#include "Com_Renderer_ParticleSystem.h"



#include "../../../Manager/TimeManager.h"
#include "../../../Manager/ResourceManager.h"

#include "../../../Resource/Mesh.h"
#include "../../../Resource/Material.h"
#include "../../../Resource/Texture.h"

#include "../../../GPU/ConstBuffer.h"
#include "../../../GPU/StructBuffer.h"

#include "../../../Game/GameObject.h"

#include "../../../json-cpp/json.h"
#include "../../../DefaultShader/Animation/Animation2D.hlsli"


#include "../Transform/Com_Transform.h"



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

	eResult Com_Renderer_ParticleSystem::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = iRenderer::SaveJson(_pJVal);

		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;


		//float4 mStartSize;
		//float4 mStartColor;

		//eSimulationSpace mSimulationSpace;
		//uint mMaxParticles;  
		//float mStartLifeTime;
		//float mFrequency;
		//float mRadius;

		//float mStartSpeed;
		//float mTime;

		////누적시간
		//float mElapsedTime;

		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mStartSize));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mStartColor));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mSimulationSpace));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mMaxParticles));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mStartLifeTime));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mFrequency));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mRadius));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mStartSpeed));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mTime));
		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mElapsedTime));

		return eResult::Success;
	}

	eResult Com_Renderer_ParticleSystem::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		//부모클래스의 LoadJson()을 호출해서 부모클래스의 데이터를 저장
		//실패시 실패결과를 리턴
		eResult result = iRenderer::LoadJson(_pJVal);

		if (eResultFail(result))
		{
			return result;
		}

		if (false == Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mStartSize)))
		{
			mStartSize = float4::One;
		}

		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mStartColor));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mSimulationSpace));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mMaxParticles));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mStartLifeTime));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mFrequency));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mRadius));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mStartSpeed));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mTime));
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mElapsedTime));

		return eResult::Success;
	}

	void Com_Renderer_ParticleSystem::Init()
	{
		using namespace strKey::defaultRes;
		mCS = LOAD_COMPUTESHADER(ParticleShader);

		std::shared_ptr<Mesh> point = ResourceManager<Mesh>::Find(mesh::PointMesh);
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = ResourceManager<Material>::Find(material::ParticleMaterial);
		SetMaterial(material, 0);

		std::shared_ptr<Texture> tex = ResourceManager<Texture>::Find(texture::CartoonSmoke);
		material->SetTexture(eTextureSlot::Albedo, tex);

		tParticle particles[100] = {};
		float4 startPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = float4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].active = 0;
			particles[i].direction =
				float4(cosf((float)i * (XM_2PI / (float)mMaxParticles))
					, sin((float)i * (XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f);

			particles[i].speed = 100.0f;
		}

		StructBuffer::Desc sDesc{};
		sDesc.eSBufferType = eStructBufferType::READ_WRITE;

		mBuffer = new StructBuffer(sDesc);
		mBuffer->Create<tParticle>(mMaxParticles, particles, 100u);

		mSharedBuffer = new StructBuffer(sDesc);
		mSharedBuffer->Create<tParticleShared>(1, nullptr, 0u);
	}

	void Com_Renderer_ParticleSystem::Update()
	{
	}

	void Com_Renderer_ParticleSystem::InternalUpdate()
	{
		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;

		//누적시간
		mTime += TimeManager::DeltaTime();
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
		float3 pos = GetOwner()->GetComponent<Com_Transform>()->GetRelativePos();
		mCBData.worldPosition = float4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.maxParticles = mMaxParticles;
		mCBData.radius = mRadius;
		mCBData.simulationSpace = (uint)mSimulationSpace;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startSize = mStartSize;
		mCBData.startColor = mStartColor;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.deltaTime = TimeManager::DeltaTime();
		mCBData.elapsedTime += TimeManager::DeltaTime();

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

		mBuffer->UnBindData();
	}
}
