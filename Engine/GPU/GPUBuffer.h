#pragma once
#include "Engine/Entity.h"

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
		GPUBuffer(const std::string_view key, eBufferType _Type);

		//버퍼까지 복사하지는 않으므로 주의할것
		GPUBuffer(const GPUBuffer& _other);

		virtual ~GPUBuffer() = default;

	protected:
		ComPtr<ID3D11Buffer>& GetBufferRef() { return m_buffer; }
		const ComPtr<ID3D11Buffer>& GetBufferRef() const { return m_buffer; }
		D3D11_BUFFER_DESC& GetBufferDescRef() { return m_bufferDesc; }
		inline void Clear();

	private:
		const eBufferType		m_bufferType;
		D3D11_BUFFER_DESC		m_bufferDesc;

		ComPtr<ID3D11Buffer>	m_buffer;
	};

	inline void GPUBuffer::Clear()
	{
		m_bufferDesc = D3D11_BUFFER_DESC{};
		m_buffer = nullptr;
	}
}


