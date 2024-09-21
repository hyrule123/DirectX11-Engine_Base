#include "Engine/Resource/Material/DebugMaterial.h"

#include "Engine/Util/AtExit.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"

namespace ehw {
	DebugMaterial::DebugMaterial()
		: Material(DebugMaterial::concrete_class_name)
		, m_debug_render_buffer()
	{
		if (nullptr == s_debug_render_buffer) {
			s_debug_render_buffer = create_debug_render_buffer();
		}
		m_debug_render_buffer = s_debug_render_buffer;
	}
	DebugMaterial::~DebugMaterial()
	{
		m_debug_render_buffer = nullptr;
		if (s_debug_render_buffer.use_count() == 1) {
			s_debug_render_buffer = nullptr;
		}
	}

	void DebugMaterial::set_data_and_bind_GPU(const std::vector<tDebugDrawData>& _data) {
		m_debug_render_buffer->set_data(_data.data(), (UINT)_data.size());
		m_debug_render_buffer->bind_data_SRV();
	}

	std::shared_ptr<StructBuffer> DebugMaterial::create_debug_render_buffer()
	{
		std::shared_ptr<StructBuffer> ret = std::make_shared<StructBuffer>();

		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_debugDrawData;
		desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		ret->init<tDebugDrawData>(desc, 100, nullptr, 0);

		return ret;
	}

}
