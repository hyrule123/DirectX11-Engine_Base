#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"


namespace ehw
{
	ParticleShader::ParticleShader()
		: ComputeShader(ParticleShader::concrete_name, uint3(128, 1, 1))
		, mBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::bind_data()
	{
		mBuffer->BindDataUAV(0);
		mSharedBuffer->BindDataUAV(1);

		uint3 dataCounts = { mBuffer->GetStride(), 1u, 1u };
		CalculateGroupCount(dataCounts);

		return true;
	}

	void ParticleShader::UnBindData()
	{
		mBuffer->unbind_data();
		mSharedBuffer->unbind_data();
	}
}
