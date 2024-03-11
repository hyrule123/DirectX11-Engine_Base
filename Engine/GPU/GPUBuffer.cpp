#include "Engine/GPU/GPUBuffer.h"
#include "Engine/Manager/GPUManager.h"

namespace ehw
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: m_bufferType(_Type)
		, m_bufferDesc{}
		, m_buffer{}
	{
	}
	GPUBuffer::GPUBuffer(const GPUBuffer& _other)
		: m_bufferType(_other.m_bufferType)
		, m_bufferDesc{_other.m_bufferDesc}
		, m_buffer{}
	{
	}

}

