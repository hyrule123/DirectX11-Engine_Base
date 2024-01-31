#pragma once
#include "../Entity.h"



#include "CommonGPU.h"

namespace ehw
{
	enum class eBufferType
	{
		Const,
		Struct,
		UnknownType,
	};

	class GPUBuffer
		: public Entity
	{
	public:
		GPUBuffer(eBufferType _Type);

		GPUBuffer(const GPUBuffer& _other);

		virtual ~GPUBuffer() = default;

	protected:
		ComPtr<ID3D11Buffer>	mBuffer;
		D3D11_BUFFER_DESC		mBufferDesc;

	private:
		eBufferType mBufferType;
	};

}


