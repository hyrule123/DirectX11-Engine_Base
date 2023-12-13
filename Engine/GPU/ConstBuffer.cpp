
#include "ConstBuffer.h"



#include "../Manager/GPUManager.h"

namespace ehw
{
	ConstBuffer::ConstBuffer(eCBType _type)
		: GPUBuffer(eBufferType::Const)
		, m_ComCategory(_type)
		, mDataSize()
		, mDataCount()
		, mPresetTargetStage(eShaderStageFlag::ALL)
	{

	}

	ConstBuffer::~ConstBuffer()
	{
	}

	bool ConstBuffer::Create(size_t _dataSize, uint _dataCount)
	{
		mDataSize = (uint)_dataSize;
		mDataCount = _dataCount;

		// 상수 버퍼
		mBufferDesc.ByteWidth = mDataSize * _dataCount;
		mBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		mBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		mBufferDesc.MiscFlags = 0;
		mBufferDesc.StructureByteStride = 0;

		bool bResult = SUCCEEDED(GPUManager::Device()->CreateBuffer(&mBufferDesc, nullptr, mBuffer.GetAddressOf()));
			
		if (false == bResult)
		{
			mBufferDesc = {};
			mBuffer = nullptr;
		}

		return bResult;
	}

	void ConstBuffer::SetData(void* _data, uint _dataCount)
	{
		ASSERT(nullptr != _data, "data가 nullptr 입니다.");
		ASSERT(_dataCount <= mDataCount, "입력된 data가 상수버퍼의 최대 data size보다 큽니다.");
	

		auto pContext = GPUManager::Context();
		D3D11_MAPPED_SUBRESOURCE tSubRes{};

		if (SUCCEEDED(pContext->Map(mBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
		{
			memcpy(tSubRes.pData, _data, mDataSize * _dataCount);
			pContext->Unmap(mBuffer.Get(), 0);
		}
	}

	void ConstBuffer::BindData(eShaderStageFlag_ _stageFlag)
	{
		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = mPresetTargetStage;
		}

		auto pContext = GPUManager::Context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetConstantBuffers((uint)m_ComCategory, 1u, mBuffer.GetAddressOf());
		}
	}
}
