#include "../PCH_Engine.h"
#include "GPUBuffer.h"



#include "../Manager/GPUManager.h"

namespace ehw
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: mBuffer()
		, mBufferDesc{}
		, mBufferType(_Type)
	{
	}
}

