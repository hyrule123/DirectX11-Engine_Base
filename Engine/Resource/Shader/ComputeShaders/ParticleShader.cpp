#include "../../../Manager/TimeManager.h"
#include "../../../Manager/RenderManager.h"

#include "../../../Resource/Shader/ComputeShaders/ParticleShader.h"

#include "../../../GPU/ConstBuffer.h"


namespace ehw
{
	ParticleShader::ParticleShader()
		: iComputeShader(typeid(ParticleShader), uint3(128, 1, 1))
		, mBuffer(nullptr)
		, mSharedBuffer(nullptr)
	{
	}

	ParticleShader::~ParticleShader()
	{
	}

	bool ParticleShader::BindData()
	{
		mBuffer->BindDataUAV(0);
		mSharedBuffer->BindDataUAV(1);

		uint3 dataCounts = { mBuffer->GetStride(), 1u, 1u };
		CalculateGroupCount(dataCounts);

		return true;
	}

	void ParticleShader::UnBindData()
	{
		mBuffer->UnBindData();
		mSharedBuffer->UnBindData();
	}
}
