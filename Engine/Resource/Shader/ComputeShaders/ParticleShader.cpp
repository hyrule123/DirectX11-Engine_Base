
#include "ParticleShader.h"

#include <Engine/Manager/TimeManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/Resource/Shader/ComputeShaders/ParticleShader.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/StructBuffer.h>


namespace core
{
	ParticleShader::ParticleShader()
		: ComputeShader(uint3(128, 1, 1))
		, m_buffer(nullptr)
		, m_shared_buffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::bind_buffer_to_GPU_register()
	{
		m_buffer->bind_buffer_as_UAV(0);
		m_shared_buffer->bind_buffer_as_UAV(1);

		uint3 dataCounts = { m_buffer->get_stride(), 1u, 1u };
		calculate_group_count(dataCounts);

		return true;
	}

	void ParticleShader::unbind_buffer_from_GPU_register()
	{
		m_buffer->unbind_buffer();
		m_shared_buffer->unbind_buffer();
	}
}
