
#include "EnginePCH.h"

#include "Com_Renderer_ParticleSystem.h"
#include "Mesh.h"
#include "ResMgr.h"
#include "Material.h"
#include "StructBuffer.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "Texture.h"
#include "TimeMgr.h"

#include "ConstBuffer.h"

#include "json-cpp/json.h"

namespace mh
{
	using namespace mh;

	Com_Renderer_ParticleSystem::Com_Renderer_ParticleSystem()
		: mMaxParticles(100)
		, mStartSize(Vector4(50.0f, 50.0f, 1.0f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.2f, 0.2f, 1.0f))
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
		: IRenderer(_other)
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
		eResult result = IRenderer::SaveJson(_pJVal);

		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;


		//Vector4 mStartSize;
		//Vector4 mStartColor;

		//eSimulationSpace mSimulationSpace;
		//UINT mMaxParticles;  
		//float mStartLifeTime;
		//float mFrequency;
		//float mRadius;

		//float mStartSpeed;
		//float mTime;

		////누적시간
		//float mElapsedTime;

		Json::MHSaveValue(_pJVal, JSONVAL(mStartSize));
		Json::MHSaveValue(_pJVal, JSONVAL(mStartColor));
		Json::MHSaveValue(_pJVal, JSONVAL(mSimulationSpace));
		Json::MHSaveValue(_pJVal, JSONVAL(mMaxParticles));
		Json::MHSaveValue(_pJVal, JSONVAL(mStartLifeTime));
		Json::MHSaveValue(_pJVal, JSONVAL(mFrequency));
		Json::MHSaveValue(_pJVal, JSONVAL(mRadius));
		Json::MHSaveValue(_pJVal, JSONVAL(mStartSpeed));
		Json::MHSaveValue(_pJVal, JSONVAL(mTime));
		Json::MHSaveValue(_pJVal, JSONVAL(mElapsedTime));

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
		eResult result = IRenderer::LoadJson(_pJVal);

		if (eResultFail(result))
		{
			return result;
		}

		if (false == Json::MHLoadValue(_pJVal, JSONVAL(mStartSize)))
		{
			mStartSize = Vector4::One;
		}

		Json::MHLoadValue(_pJVal, JSONVAL(mStartColor));
		Json::MHLoadValue(_pJVal, JSONVAL(mSimulationSpace));
		Json::MHLoadValue(_pJVal, JSONVAL(mMaxParticles));
		Json::MHLoadValue(_pJVal, JSONVAL(mStartLifeTime));
		Json::MHLoadValue(_pJVal, JSONVAL(mFrequency));
		Json::MHLoadValue(_pJVal, JSONVAL(mRadius));
		Json::MHLoadValue(_pJVal, JSONVAL(mStartSpeed));
		Json::MHLoadValue(_pJVal, JSONVAL(mTime));
		Json::MHLoadValue(_pJVal, JSONVAL(mElapsedTime));

		return eResult::Success;
	}

	void Com_Renderer_ParticleSystem::Init()
	{
		using namespace strKey::Default;
		mCS = ResMgr::Find<ParticleShader>(shader::compute::ParticleCS);

		std::shared_ptr<Mesh> point = ResMgr::Find<Mesh>(mesh::PointMesh);
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = ResMgr::Find<Material>(material::ParticleMaterial);
		SetMaterial(material);

		std::shared_ptr<Texture> tex = ResMgr::Find<Texture>(texture::CartoonSmoke);
		material->SetTexture(eTextureSlot::Albedo, tex);

		tParticle particles[100] = {};
		Vector4 startPos = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = Vector4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].active = 0;
			particles[i].direction =
				Vector4(cosf((float)i * (XM_2PI / (float)mMaxParticles))
					, sin((float)i * (XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f);

			particles[i].speed = 100.0f;
		}

		tSBufferDesc sDesc{};
		sDesc.eSBufferType = eStructBufferType::READ_WRITE;

		mBuffer = new StructBuffer(sDesc);
		mBuffer->Create<tParticle>(mMaxParticles, particles, 100u);

		mSharedBuffer = new StructBuffer(sDesc);
		mSharedBuffer->Create<tParticleShared>(1, nullptr, 0u);
	}

	void Com_Renderer_ParticleSystem::Update()
	{
	}

	void Com_Renderer_ParticleSystem::FixedUpdate()
	{
		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;

		//누적시간
		mTime += TimeMgr::DeltaTime();
		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			UINT iAliveCount = (UINT)f;
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
		Vector3 pos = GetOwner()->GetTransform().GetPosition();
		mCBData.WorldPosition = Vector4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.MaxParticles = mMaxParticles;
		mCBData.Radius = mRadius;
		mCBData.SimulationSpace = (UINT)mSimulationSpace;
		mCBData.StartSpeed = mStartSpeed;
		mCBData.StartSize = mStartSize;
		mCBData.StartColor = mStartColor;
		mCBData.StartLifeTime = mStartLifeTime;
		mCBData.DeltaTime = TimeMgr::DeltaTime();
		mCBData.ElapsedTime += TimeMgr::DeltaTime();

		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::ParticleSystem);
		cb->SetData(&mCBData);
		cb->BindData(eShaderStageFlag::ALL);

		mCS->SetSharedStrutedBuffer(mSharedBuffer);
		mCS->SetStrcutedBuffer(mBuffer);
		mCS->OnExcute();
	}

	void Com_Renderer_ParticleSystem::Render()
	{
		GetOwner()->GetTransform().SetConstBuffer();
		mBuffer->BindDataSRV(15, eShaderStageFlag::GS);

		GetMaterial()->Bind();
		GetMesh()->RenderInstanced(mMaxParticles);

		mBuffer->UnBind();
	}
}
