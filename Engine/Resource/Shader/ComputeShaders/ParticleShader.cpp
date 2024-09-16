#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"


namespace ehw
{
	ParticleShader::ParticleShader()
		: ComputeShader(ParticleShader::concrete_class_name, uint3(128, 1, 1))
		, mBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::bind_buffer_to_GPU_register()
	{
		mBuffer->BindDataUAV(0);
		mSharedBuffer->BindDataUAV(1);

		uint3 dataCounts = { mBuffer->GetStride(), 1u, 1u };
		calculate_group_count(dataCounts);

		return true;
	}

	void ParticleShader::unbind_buffer_from_GPU_register()
	{
		mBuffer->unbind_data();
		mSharedBuffer->unbind_data();
	}
}
