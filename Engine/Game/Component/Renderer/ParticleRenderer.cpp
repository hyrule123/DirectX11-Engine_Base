#include "ParticleRenderer.h"

#include <Engine/Game/Component/Transform.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Manager/TimeManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Texture.h>
#include <Engine/Resource/Shader/ComputeShaders/ParticleShader.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/StructBuffer.h>

#include <Engine/DefaultShader/Animation/Animation2D.hlsli>

namespace core
{
	ParticleRenderer::ParticleRenderer()
		: StaticMeshRenderer()
		, m_max_particles(100)
		, m_start_size(float4(50.0f, 50.0f, 1.0f, 1.0f))
		, m_start_color(float4(1.0f, 0.2f, 0.2f, 1.0f))
		, m_start_lifetime(3.0f)
		, m_frequency(1.0f)
		, m_time(0.0f)
		, mCBData{}
		, m_simulation_space(eSimulationSpace::World)
		, m_radius(500.0f)
		, m_start_speed(200.0f)
		, m_elapsed_time(0.0f)

	{

	}

	ParticleRenderer::ParticleRenderer(const ParticleRenderer& _other)
		: StaticMeshRenderer(_other)
		, m_max_particles(_other.m_max_particles)
		, m_start_size(_other.m_start_size)
		, m_start_color(_other.m_start_color)
		, m_start_lifetime(_other.m_start_lifetime)
		, m_frequency(_other.m_frequency)
		, m_time(_other.m_time)
		, mCBData{ _other.mCBData }
		, m_simulation_space(_other.m_simulation_space)
		, m_radius(_other.m_radius)
		, m_start_speed(_other.m_start_speed)
		, m_elapsed_time(_other.m_elapsed_time)
	{
		if (_other.m_buffer != nullptr)
		{
			//m_buffer = _other.m_buffer->clone();
		}
		if (_other.m_shared_buffer != nullptr)
		{
			//m_shared_buffer = _other.m_shared_buffer->clone();
		}


	}

	ParticleRenderer::~ParticleRenderer()
	{
	}

	void ParticleRenderer::init()
	{
		using namespace name::defaultRes;
		m_particle_CS = LOAD_COMPUTESHADER(ParticleShader);

		s_ptr<Mesh> point = ResourceManager<Mesh>::get_inst().find(mesh::PointMesh);
		set_mesh(point);

		// Material 세팅
		s_ptr<Material> material = ResourceManager<Material>::get_inst().find(material::ParticleMaterial);
		set_material(material);

		s_ptr<Texture> tex = ResourceManager<Texture>::get_inst().find(texture::CartoonSmoke);
		material->set_texture(eTextureSlot::diffuse_texture, tex);

		tParticle particles[100] = {};
		float4 startPos = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < m_max_particles; i++)
		{
			particles[i].position = float4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].active = 0;
			particles[i].direction =
				float4(cosf((float)i * (DirectX::XM_2PI / (float)m_max_particles))
					, sin((float)i * (DirectX::XM_2PI / (float)m_max_particles)), 0.0f, 1.0f);

			particles[i].speed = 100.0f;
		}

		StructBuffer::tDesc sDesc{};
		sDesc.m_buffer_RW_type = eStructBufferType::READ_WRITE;

		m_buffer = new_entity<StructBuffer>();
		m_buffer->create<tParticle>(sDesc, m_max_particles, particles, 100u);

		m_shared_buffer = new_entity<StructBuffer>();
		m_shared_buffer->create<tParticleShared>(sDesc, 1, nullptr, 0u);
	}

	void ParticleRenderer::final_update()
	{
		Renderer::final_update();

		//파티클 생성 시간
		float aliveTime = 1.0f / m_frequency;

		//누적시간
		m_time += TimeManager::get_inst().delta_time();
		if (aliveTime < m_time)
		{
			float f = (m_time / aliveTime);
			uint iAliveCount = (uint)f;
			m_time = f - std::floor(f);

			tParticleShared shared = { 5, };
			m_shared_buffer->set_data(&shared, 1);
		}
		else
		{
			tParticleShared shared = {  };
			m_shared_buffer->set_data(&shared, 1);
		}

		m_max_particles = m_buffer->get_stride();
		float3 pos = get_owner()->get_component<Transform>()->get_local_position();
		mCBData.worldPosition = float4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.maxParticles = m_max_particles;
		mCBData.radius = m_radius;
		mCBData.simulationSpace = (uint)m_simulation_space;
		mCBData.startSpeed = m_start_speed;
		mCBData.startSize = m_start_size;
		mCBData.startColor = m_start_color;
		mCBData.startLifeTime = m_start_lifetime;
		mCBData.deltaTime = TimeManager::get_inst().delta_time();
		mCBData.elapsedTime += TimeManager::get_inst().delta_time();


		ConstBuffer* cb = RenderManager::get_inst().get_const_buffer(eCBType::ParticleSystem);
		cb->set_data(&mCBData);
		cb->bind_buffer_to_GPU_register(eShaderStageFlag::ALL);

		m_particle_CS->set_shared_struct_buffer(m_shared_buffer);
		m_particle_CS->set_struct_buffer(m_buffer);
		m_particle_CS->on_execute();
	}

	//void ParticleRenderer::render()
	//{
	//	if (false == is_render_ready())
	//		return;
	//	
	//	get_owner()->get_component<Transform>()->bind_buffer_to_GPU_register();
	//	m_buffer->bind_buffer_as_SRV(GPU::Register::t::g_diffuse_texture, eShaderStageFlag::Geometry);

	//	get_current_material(0)->bind_buffer_to_gpu_register();
	//	get_mesh()->render_instanced(m_max_particles);

	//	m_buffer->unbind_buffer();
	//}
}
