
#include "GPUBuffer.h"

#include <Engine/Manager/RenderManager.h>

namespace core
{
	GPUBuffer::GPUBuffer(eBufferType _Type)
		: Resource()
		, m_buffer_type(_Type)
	{
	}
}

