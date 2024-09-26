#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/GPU/ConstBuffer.h"

namespace ehw {
	void RenderManager::create_const_buffers()
	{
#pragma region CONSTANT BUFFER
		m_constBuffers[(uint)eCBType::Global] = std::make_unique<ConstBuffer>((uint)eCBType::Global);
		m_constBuffers[(uint)eCBType::Global]->create(sizeof(tCB_Global));
		m_constBuffers[(uint)eCBType::Global]->SetPresetTargetStage(eShaderStageFlag::ALL);

		UpdateGlobalCBuffer();

		m_constBuffers[(uint)eCBType::Camera] = std::make_unique<ConstBuffer>((uint)eCBType::Camera);
		m_constBuffers[(uint)eCBType::Camera]->create<tCamera>();

		m_constBuffers[(uint)eCBType::ComputeShader] = std::make_unique<ConstBuffer>((uint)eCBType::ComputeShader);
		m_constBuffers[(uint)eCBType::ComputeShader]->create<tCB_ComputeShader>();
		m_constBuffers[(uint)eCBType::ComputeShader]->SetPresetTargetStage(eShaderStageFlag::Compute);

		m_constBuffers[(uint)eCBType::Material] = std::make_unique<ConstBuffer>((uint)eCBType::Material);
		m_constBuffers[(uint)eCBType::Material]->create(sizeof(tSharedMaterialData));

		m_constBuffers[(uint)eCBType::Animation2D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation2D);
		m_constBuffers[(uint)eCBType::Animation2D]->create(sizeof(tCB_Animation2D));

		m_constBuffers[(uint)eCBType::ParticleSystem] = std::make_unique<ConstBuffer>((uint)eCBType::ParticleSystem);
		m_constBuffers[(uint)eCBType::ParticleSystem]->create(sizeof(tCB_ParticleSystem));

		m_constBuffers[(uint)eCBType::Noise] = std::make_unique<ConstBuffer>((uint)eCBType::Noise);
		m_constBuffers[(uint)eCBType::Noise]->create(sizeof(tCB_Noise));

		m_constBuffers[(uint)eCBType::Animation3D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation3D);
		m_constBuffers[(uint)eCBType::Animation3D]->create<tAnimation3D_ComputeShaderData>();

#pragma endregion
	}
}
