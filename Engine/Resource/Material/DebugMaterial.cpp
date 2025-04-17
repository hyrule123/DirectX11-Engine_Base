#include "Engine/Resource/Material/DebugMaterial.h"

#include "Engine/Util/AtExit.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"

#include "Engine/Manager/ResourceManager.h"

namespace core {
	DebugMaterial::DebugMaterial()
		: Material(DebugMaterial::concrete_class_name)
		, m_debug_render_buffer()
	{
		m_debug_render_buffer = ResourceManager<StructBuffer>::get_inst().find("debug_render_buffer");
	}
	DebugMaterial::~DebugMaterial()
	{
	}

	void DebugMaterial::set_data_and_bind_GPU(const std::vector<tDebugDrawData>& _data) {
		m_debug_render_buffer->set_data(_data.data(), (UINT)_data.size());
		m_debug_render_buffer->bind_buffer_as_SRV();
	}
}
