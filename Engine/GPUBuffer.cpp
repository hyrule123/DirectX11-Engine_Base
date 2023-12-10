#include "PCH_Engine.h"

#include "GPUBuffer.h"
#include "GPUManager.h"

namespace ehw
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: mBuffer()
		, mBufferDesc{}
		, mBufferType(_Type)
	{
	}
}

