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
		GPUBuffer(const std::string_view key, eBufferType _Type);

		//버퍼까지 복사하지는 않으므로 주의할것
		GPUBuffer(const GPUBuffer& _other);

		virtual ~GPUBuffer() = default;

		eBufferType get_buffer_type() const { return m_buffer_type; }

		const D3D11_BUFFER_DESC& get_buffer_desc() const { return m_buffer_desc; }
		ComPtr<ID3D11Buffer> get_buffer() const { return m_buffer; }

	protected:
		void clear();

		//getset을 통해서 해야하는 이유
		//derived class에서 멤버변수에 바로 접근해서 데이터를 변경할 경우 혹시나 버퍼 생성 실패 시 예외 처리가 힘들어진다.
		void set_buffer(const ComPtr<ID3D11Buffer>& _buf) { m_buffer = _buf; }
		void set_buffer_desc(const D3D11_BUFFER_DESC& _desc) { m_buffer_desc = _desc; }

	private:
		const eBufferType		m_buffer_type;

		ComPtr<ID3D11Buffer>	m_buffer;
		D3D11_BUFFER_DESC		m_buffer_desc;
	};


}


