#include "Engine/GPU/GPUBuffer.h"
#include "Engine/Manager/RenderManager.h"

namespace core
{
	GPUBuffer::GPUBuffer(const std::string_view key, eBufferType _Type)
		: Resource(key)
		, m_bufferType(_Type)
		, m_bufferDesc{}
		, m_buffer{}
	{
	}
	GPUBuffer::GPUBuffer(const GPUBuffer& _other)
		: Resource(_other)
		, m_bufferType(_other.m_bufferType)
		, m_bufferDesc{_other.m_bufferDesc}
		, m_buffer{}
	{
	}

}

