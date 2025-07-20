
#include "ConstBuffer.h"
#include <Engine/Manager/RenderManager.h>

namespace core
{
	ConstBuffer::ConstBuffer(uint _type)
		: GPUBuffer(ConstBuffer::s_static_type_name, eBufferType::Const)
		, m_const_buffer_type(_type)
		, m_data_size()
		, m_data_count()
		, m_target_stage(eShaderStageFlag::ALL)
	{
	}

	ConstBuffer::~ConstBuffer()
	{
	}

	bool ConstBuffer::create(size_t _dataSize, uint _dataCount)
	{
		m_data_size = (uint)_dataSize;
		m_data_count = _dataCount;

		// 상수 버퍼
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = m_data_size * _dataCount;
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		
		ComPtr<ID3D11Buffer> buf = nullptr;

		HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&buffer_desc, nullptr, buf.GetAddressOf());
		if (FAILED(result))
		{
			ERROR_MESSAGE("상수버퍼 생성 실패.");
			return false;
		}

		set_buffer(buf);
		set_buffer_desc(buffer_desc);

		return true;
	}

	void ConstBuffer::set_data(const void* _data, uint _dataCount)
	{
		ASSERT(nullptr != _data, "data가 nullptr 입니다.");
		ASSERT(_dataCount <= m_data_count, "입력된 data가 상수버퍼의 최대 data size보다 큽니다.");
		

		auto pContext = RenderManager::get_inst().get_context();
		D3D11_MAPPED_SUBRESOURCE tSubRes{};

		if (SUCCEEDED(pContext->Map(get_buffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
		{
			memcpy(tSubRes.pData, _data, m_data_size * _dataCount);
			pContext->Unmap(get_buffer().Get(), 0);
		}
	}

	void ConstBuffer::bind_buffer_to_GPU_register(eShaderStageFlag_ _stageFlag)
	{
		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = m_target_stage;
		}

		auto pContext = RenderManager::get_inst().get_context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetConstantBuffers(m_const_buffer_type, 1u, get_buffer().GetAddressOf());
		}
	}
	void ConstBuffer::unbind_buffer()
	{
		auto pContext = RenderManager::get_inst().get_context();

		pContext->VSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
		pContext->HSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
		pContext->DSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
		pContext->GSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
		pContext->PSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
		pContext->CSSetConstantBuffers(m_const_buffer_type, 0u, nullptr);
	}
}
