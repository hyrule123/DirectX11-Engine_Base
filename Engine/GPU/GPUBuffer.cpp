
#include "GPUBuffer.h"



#include "../Manager/GPUManager.h"

namespace ehw
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: mBuffer{}
		, mBufferDesc{}
		, mBufferType(_Type)
	{
	}
	GPUBuffer::GPUBuffer(const GPUBuffer& _other)
		: mBuffer()
		, mBufferDesc{_other.mBufferDesc}
		, mBufferType{_other.mBufferType}
	{
	}
}

