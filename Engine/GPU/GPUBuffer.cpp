
#include "GPUBuffer.h"

#include <Engine/Manager/RenderManager.h>

namespace core
{
	GPUBuffer::GPUBuffer(const std::string_view key, eBufferType _Type)
		: Resource(key)
		, m_buffer_type(_Type)
		, m_buffer_desc{}
		, m_buffer{}
	{
	}
	GPUBuffer::GPUBuffer(const GPUBuffer& _other)
		: Resource(_other)
		, m_buffer_type(_other.m_buffer_type)
		, m_buffer_desc{_other.m_buffer_desc}
		, m_buffer{}
	{
	}

	void GPUBuffer::clear()
	{
		m_buffer_desc = D3D11_BUFFER_DESC{};
		m_buffer = nullptr;
	}
}

