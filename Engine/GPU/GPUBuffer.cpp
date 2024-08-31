#include "Engine/GPU/GPUBuffer.h"
#include "Engine/Manager/RenderManager.h"

namespace ehw
{
	GPUBuffer::GPUBuffer(const std::string_view key, eBufferType _Type)
		: Entity(key)
		, m_bufferType(_Type)
		, m_bufferDesc{}
		, m_buffer{}
	{
	}
	GPUBuffer::GPUBuffer(const GPUBuffer& _other)
		: Entity(_other)
		, m_bufferType(_other.m_bufferType)
		, m_bufferDesc{_other.m_bufferDesc}
		, m_buffer{}
	{
	}

}

