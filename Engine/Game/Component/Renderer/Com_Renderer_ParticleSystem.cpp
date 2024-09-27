#include "Engine/Game/Component/Renderer/Com_Renderer_ParticleSystem.h"

#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"
#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Animation/Animation2D.hlsli"

namespace ehw
{
	Com_Renderer_ParticleSystem::Com_Renderer_ParticleSystem()
		: Com_Renderer_Mesh(Com_Renderer_ParticleSystem::concrete_class_name)
		, mMaxParticles(100)
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
		if (_other.m_buffer != nullptr)
		{
			//m_buffer = _other.m_buffer->Clone();
		}
		if (_other.m_shared_buffer != nullptr)
		{
			//m_shared_buffer = _other.m_shared_buffer->Clone();
		}


	}

	Com_Renderer_ParticleSystem::~Com_Renderer_ParticleSystem()
	{
		delete m_buffer;
		m_buffer = nullptr;

		delete m_shared_buffer;
		m_shared_buffer = nullptr;
	}


	eResult Com_Renderer_ParticleSystem::serialize_json(JsonSerializer* Json) const
	{
		return eResult();
	}

	eResult Com_Renderer_ParticleSystem::deserialize_json(const JsonSerializer* Json)
	{
		return eResult();
	}

	void Com_Renderer_ParticleSystem::init()
	{
		using namespace name::defaultRes;
		mCS = LOAD_COMPUTESHADER(ParticleShader);

		std::shared_ptr<Mesh> point = ResourceManager<Mesh>::get_inst().find(mesh::PointMesh);
		set_mesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = ResourceManager<Material>::get_inst().find(material::ParticleMaterial);
		set_material(material);

		std::shared_ptr<Texture> tex = ResourceManager<Texture>::get_inst().find(texture::CartoonSmoke);
		material->set_texture(eTextureSlot::Albedo, tex);

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

		m_buffer = new StructBuffer;
		m_buffer->init<tParticle>(sDesc, mMaxParticles, particles, 100u);

		m_shared_buffer = new StructBuffer;
		m_shared_buffer->init<tParticleShared>(sDesc, 1, nullptr, 0u);
	}

	void Com_Renderer_ParticleSystem::final_update()
	{
		Renderer::final_update();

		//파티클 생성 시간
		float aliveTime = 1.0f / mFrequency;

		//누적시간
		mTime += TimeManager::get_inst().DeltaTime();
		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			uint iAliveCount = (uint)f;
			mTime = f - std::floor(f);

			tParticleShared shared = { 5, };
			m_shared_buffer->set_data(&shared, 1);
		}
		else
		{
			tParticleShared shared = {  };
			m_shared_buffer->set_data(&shared, 1);
		}

		mMaxParticles = m_buffer->GetStride();
		float3 pos = gameObject()->GetComponent<Transform>()->get_local_position();
		mCBData.worldPosition = float4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.maxParticles = mMaxParticles;
		mCBData.radius = mRadius;
		mCBData.simulationSpace = (uint)mSimulationSpace;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startSize = mStartSize;
		mCBData.startColor = mStartColor;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.deltaTime = TimeManager::get_inst().DeltaTime();
		mCBData.elapsedTime += TimeManager::get_inst().DeltaTime();


		ConstBuffer* cb = RenderManager::get_inst().get_const_buffer(eCBType::ParticleSystem);
		cb->set_data(&mCBData);
		cb->bind_buffer_to_GPU_register(eShaderStageFlag::ALL);

		mCS->SetSharedStrutedBuffer(m_shared_buffer);
		mCS->SetStrcutedBuffer(m_buffer);
		mCS->on_execute();
	}

	//void Com_Renderer_ParticleSystem::render()
	//{
	//	if (false == IsRenderReady())
	//		return;
	//	
	//	gameObject()->GetComponent<Transform>()->bind_buffer_to_GPU_register();
	//	m_buffer->bind_buffer_as_SRV(GPU::Register::t::AlbedoTexture, eShaderStageFlag::Geometry);

	//	GetCurrentMaterial(0)->bind_buffer_to_gpu_register();
	//	GetMesh()->render_instanced(mMaxParticles);

	//	m_buffer->unbind_buffer();
	//}
}
