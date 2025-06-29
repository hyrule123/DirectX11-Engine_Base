#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	ConstBuffer::ConstBuffer(uint _type)
		: GPUBuffer(ConstBuffer::s_static_type_name, eBufferType::Const)
		, m_constBufferType(_type)
		, mDataSize()
		, mDataCount()
		, mPresetTargetStage(eShaderStageFlag::ALL)
	{
	}

	ConstBuffer::~ConstBuffer()
	{
	}

	bool ConstBuffer::create(size_t _dataSize, uint _dataCount)
	{
		mDataSize = (uint)_dataSize;
		mDataCount = _dataCount;

		// 상수 버퍼
		D3D11_BUFFER_DESC& bufferDesc = GetBufferDescRef();
		bufferDesc.ByteWidth = mDataSize * _dataCount;
		bufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		bool bResult = SUCCEEDED(RenderManager::get_inst().Device()->CreateBuffer(&GetBufferDescRef(), nullptr, GetBufferRef().GetAddressOf()));
		if (false == bResult)
		{
			Clear();
		}

		return bResult;
	}

	void ConstBuffer::set_data(const void* _data, uint _dataCount)
	{
		ASSERT(nullptr != _data, "data가 nullptr 입니다.");
		ASSERT(_dataCount <= mDataCount, "입력된 data가 상수버퍼의 최대 data size보다 큽니다.");
	

		auto pContext = RenderManager::get_inst().Context();
		D3D11_MAPPED_SUBRESOURCE tSubRes{};

		if (SUCCEEDED(pContext->Map(GetBufferRef().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
		{
			memcpy(tSubRes.pData, _data, mDataSize * _dataCount);
			pContext->Unmap(GetBufferRef().Get(), 0);
		}
	}

	void ConstBuffer::bind_buffer_to_GPU_register(eShaderStageFlag_ _stageFlag)
	{
		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = mPresetTargetStage;
		}

		auto pContext = RenderManager::get_inst().Context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetConstantBuffers(m_constBufferType, 1u, GetBufferRef().GetAddressOf());
		}
	}
	void ConstBuffer::unbind_buffer()
	{
		auto pContext = RenderManager::get_inst().Context();

		pContext->VSSetConstantBuffers(m_constBufferType, 0u, nullptr);
		pContext->HSSetConstantBuffers(m_constBufferType, 0u, nullptr);
		pContext->DSSetConstantBuffers(m_constBufferType, 0u, nullptr);
		pContext->GSSetConstantBuffers(m_constBufferType, 0u, nullptr);
		pContext->PSSetConstantBuffers(m_constBufferType, 0u, nullptr);
		pContext->CSSetConstantBuffers(m_constBufferType, 0u, nullptr);
	}
}
