#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"


namespace core
{
	ParticleShader::ParticleShader()
		: ComputeShader(ParticleShader::s_static_type_name, uint3(128, 1, 1))
		, mBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::bind_buffer_to_GPU_register()
	{
		mBuffer->bind_buffer_as_UAV(0);
		mSharedBuffer->bind_buffer_as_UAV(1);

		uint3 dataCounts = { mBuffer->GetStride(), 1u, 1u };
		calculate_group_count(dataCounts);

		return true;
	}

	void ParticleShader::unbind_buffer_from_GPU_register()
	{
		mBuffer->unbind_buffer();
		mSharedBuffer->unbind_buffer();
	}
}
