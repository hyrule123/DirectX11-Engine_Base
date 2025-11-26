
#include "ConstBuffer.h"
#include <Engine/Manager/RenderManager.h>

namespace core
{
	ConstBuffer::ConstBuffer()
		: GPUBuffer(eBufferType::Const)
	{
		m_target_slot = std::numeric_limits<UINT>::max();
	}

	ConstBuffer::~ConstBuffer()
	{
		unbind();
	}

	bool ConstBuffer::create(UINT _target_register, UINT _dataSize, UINT _dataCount)
	{
		m_target_slot = _target_register;

		m_data_size = _dataSize;
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

		m_buffer = buf;
		m_buffer_desc = buffer_desc;

		return true;
	}

	void ConstBuffer::set_data(const void* _data, uint _dataCount)
	{
		ASSERT_MESSAGE(nullptr != _data, "data가 nullptr 입니다.");
		ASSERT_MESSAGE(_dataCount <= m_data_count, "입력된 data가 상수버퍼의 최대 data size보다 큽니다.");

		auto pContext = RenderManager::get_inst().get_context();
		D3D11_MAPPED_SUBRESOURCE tSubRes{};

		HRESULT result = pContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubRes);
		if (SUCCEEDED(result))
		{
			memcpy(tSubRes.pData, _data, m_data_size * _dataCount);
			pContext->Unmap(m_buffer.Get(), 0);
		}
	}

	void ConstBuffer::bind(eShaderStageFlag_ _stage_flag)
	{
		auto pContext = RenderManager::get_inst().get_context();

		if (eShaderStageFlag::Vertex & _stage_flag)
		{
			pContext->VSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Vertex][m_target_slot] = m_buffer;
		}
		if (eShaderStageFlag::Hull & _stage_flag)
		{
			pContext->HSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Hull][m_target_slot] = m_buffer;
		}
		if (eShaderStageFlag::Domain & _stage_flag)
		{
			pContext->DSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Domain][m_target_slot] = m_buffer;
		}
		if (eShaderStageFlag::Geometry & _stage_flag)
		{
			pContext->GSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Geometry][m_target_slot] = m_buffer;
		}
		if (eShaderStageFlag::Pixel & _stage_flag)
		{
			pContext->PSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Pixel][m_target_slot] = m_buffer;
		}
		if (eShaderStageFlag::Compute & _stage_flag)
		{
			pContext->CSSetConstantBuffers(m_target_slot, 1, m_buffer.GetAddressOf());
			s_bound_table[(int)eShaderStage::Compute][m_target_slot] = m_buffer;
		}
	}
	void ConstBuffer::unbind()
	{
		auto pContext = RenderManager::get_inst().get_context();

		for (int i = 0; i < (int)eShaderStage::END; ++i)
		{
			if (s_bound_table[i][m_target_slot].Get() == m_buffer.Get())
			{
				s_bound_table[i][m_target_slot] = nullptr;
				
				switch ((eShaderStage)i)
				{
				case core::eShaderStage::Vertex:
					pContext->VSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::Hull:
					pContext->HSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::Domain:
					pContext->DSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::Geometry:
					pContext->GSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::Pixel:
					pContext->PSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::Compute:
					pContext->CSSetConstantBuffers(m_target_slot, 1, nullptr);
					break;
				case core::eShaderStage::END:
					break;
				default:
					break;
				}
			}
		}
	}
	eResult ConstBuffer::save(const std::fs::path& _fullpath) const
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}
	eResult ConstBuffer::load(const std::fs::path& _fullpath)
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}
}
