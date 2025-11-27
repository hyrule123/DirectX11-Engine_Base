#pragma once
#include <Engine/Resource/Resource.h>

#include <Engine/GPU/CommonGPU.h>

//사용법: 직접 만들어서 register_buffer에 생성하는데 만든 description과 함께 등록
//만들어주지 않음.

namespace core
{
	enum class eBufferType
	{
		Const,
		Struct,
		UnknownType,
	};

	class GPUBuffer
		: public Resource
	{
		CLASS_INFO(GPUBuffer, Resource);
	public:
		GPUBuffer(eBufferType _Type);
		virtual ~GPUBuffer() = default;

		eBufferType get_buffer_type() const { return m_buffer_type; }

	protected:
		const eBufferType		m_buffer_type;

		ComPtr<ID3D11Buffer>	m_buffer = {};
		D3D11_BUFFER_DESC		m_buffer_desc = {};
	};


}


