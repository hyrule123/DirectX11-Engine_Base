#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/RenderManager.h"


#include "Engine/GPU/ConstBuffer.h"

namespace ehw
{
	StructBuffer::StructBuffer()
		: GPUBuffer(INSTANCE_ABLE(StructBuffer), eBufferType::Struct)
		, m_desc()
		, m_elementStride()
		, m_elementCount()
		, m_capacity()
		, m_SRV()
		, m_UAV()
		, m_stagingBuffer()
		, m_curBoundView()
		, m_curBoundRegister(-1)
	{
	}

	StructBuffer::StructBuffer(const StructBuffer& _other)
		: GPUBuffer(_other)
		, m_desc()
		, m_elementStride(_other.m_elementStride)
		, m_elementCount()
		, m_capacity()
		, m_SRV()
		, m_UAV()
		, m_stagingBuffer()
		, m_curBoundView()
		, m_curBoundRegister(-1)
	{
		//SetDesc 함수를 통해 설정
		SetDesc(_other.m_desc);

		//버퍼 빈 상태 및 동일 조건으로 생성
		Resize(_other.m_capacity, nullptr, 0u);

		//리소스 내용 복사
		RenderManager::GetInst().Context()->CopyResource(GetBufferRef().Get(), _other.GetBufferRef().Get());
	}

	StructBuffer::~StructBuffer()
	{
		UnbindData();
	}

	void StructBuffer::SetDesc(const Desc& _tDesc)
	{
		m_desc = _tDesc;

		switch (m_desc.eSBufferType)
		{
		case eStructBufferType::READ_ONLY:

			//CPU에서 구조화버퍼로 작성할 수 있어야 하므로 AccessFlag를 write로 설정
			GetBufferDescRef().CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
			GetBufferDescRef().Usage = D3D11_USAGE_DYNAMIC;

			//SRV만 바인딩되도록 설정
			GetBufferDescRef().BindFlags = D3D11_BIND_SHADER_RESOURCE;

			break;
		case eStructBufferType::READ_WRITE:

			//CPU는 직접 접근 불가, 
			GetBufferDescRef().CPUAccessFlags = 0;

			//GPU의 읽기/쓰기는 가능
			GetBufferDescRef().Usage = D3D11_USAGE_DEFAULT;

			//SRV, UAV 둘 다 바인딩되도록 설정
			GetBufferDescRef().BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

			//READ_WRITE로 사용하겠다는 건 컴퓨트쉐이더를 사용하겠다는 의미 -> 실수 방지를 위해 플래그에 컴퓨트쉐이더 추가
			m_desc.TargetStageSRV |= eShaderStageFlag::Compute;

			break;
		default:
			break;
		}

		//기타 플래그에 구조화 버퍼로 설정값이 있음.
		GetBufferDescRef().MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	}

	eResult StructBuffer::Resize(size_t _capacity, const void* _pInitialData, size_t _uElemCount)
	{
		if (_capacity == 0) {
			return eResult::Success;
		}
		//이미 더 큰 capacity일 경우 그냥 그 버퍼를 사용한다
		if (_capacity <= m_capacity) {
			SetData(_pInitialData, _uElemCount);
			return eResult::Success;
		}
		else if (_capacity < _uElemCount)
		{
			ERROR_MESSAGE_A("Element capacity of structured buffer must be more than element of input data!");
			return eResult::Fail_Create;
		}

		//재할당 하기 전 바인딩된 리소스가 있다면 unbind
		UnbindData();

		//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
		m_elementCount = (uint)_uElemCount;
		m_capacity = (uint)_capacity;

		D3D11_BUFFER_DESC& bufferDesc = GetBufferDescRef();
		bufferDesc.StructureByteStride = m_elementStride;
		bufferDesc.ByteWidth = m_elementStride * m_capacity;

		switch (m_desc.eSBufferType)
		{
			//일반적인 GPU에서 읽기만 가능한 구조화 버퍼
		case eStructBufferType::READ_ONLY:
		{
			//초깃값 업로드 및 바인딩
			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			D3D11_SUBRESOURCE_DATA Data = {};
			if (nullptr != _pInitialData)
			{
				Data.pSysMem = _pInitialData;

				//이 값은 무시됨. BufferDesc.ByteWidth 값만 영향을 미치는 것을 확인함.
				Data.SysMemPitch = m_elementStride * (uint)_uElemCount;
				Data.SysMemSlicePitch = bufferDesc.StructureByteStride;
				pData = &Data;
			}


			//구조화버퍼 생성. ReleaseAndGetAddressOf() 함수를 통해서 기존 구조화 버퍼가 있다면 날려버리고 생성
			if (FAILED(RenderManager::GetInst().Device()->CreateBuffer(&bufferDesc, pData, GetBufferRef().ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return eResult::Fail_Create;
			}

			//Read Only의 경우 바로 SRV 생성
			CreateSRV();

			break;
		}

		//Compute Shader과 결합해서 사용하는, GPU에서 조작 가능한 SBuffer
		//이 경우에는 CPU와 데이터 전송이 불가능하므로 데이터 전송용 Staging Buffer를 중간 단계로 사용해야 한다.
		case eStructBufferType::READ_WRITE:
		{
			//초깃값 지정
			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			D3D11_SUBRESOURCE_DATA Data = {};
			if (nullptr != _pInitialData)
			{
				Data.pSysMem = _pInitialData;
				Data.SysMemPitch = m_elementStride * (uint)_uElemCount;
				Data.SysMemSlicePitch = bufferDesc.StructureByteStride;
				pData = &Data;
			}

			//구조화버퍼 생성
			if (FAILED(RenderManager::GetInst().Device()->CreateBuffer(&bufferDesc, pData, GetBufferRef().ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return eResult::Fail_Create;
			}


			CreateSRV();
			CreateUAV();

			break;
		}

		}

		return eResult::Success;
	}

	void StructBuffer::SetData(const void* _pData, size_t _uCount)
	{
		m_elementCount = (uint)_uCount;

		//g_arrSBufferShareData의 자신의 인덱스에 해당하는 위치에 이번에 업데이트된 구조체의 갯수를 삽입
		//상수 버퍼의 바인딩은 BindData()를 하는 시점에 해준다.
		tCB_SBufferCount cb = {};
		cb.SBufferDataCount = m_elementCount;


		//생성 시 할당된 갯수보다 들어온 갯수가 더 클 경우 재할당하고, 거기에 데이터를 추가.
		//생성될 때 값을 지정할 수 있으므로 바로 return 해주면 될듯
		if (_uCount > m_capacity)
		{
			//다시 생성하고자 할때는 초기 데이터와 사이즈를 일치시켜서 생성해줘야 한다.
			Resize(_uCount, _pData, _uCount);
			return;
		}

		auto pContext = RenderManager::GetInst().Context();
		switch (m_desc.eSBufferType)
		{
		case eStructBufferType::READ_ONLY:
		{
			{
				//Read Only일 경우 SBuffer에 바로 Map/UnMap을 해주면 된다.
				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(GetBufferRef().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(m_elementStride * _uCount));

					pContext->Unmap(GetBufferRef().Get(), 0);
				}

				break;
			}
		}

		case eStructBufferType::READ_WRITE:
		{
			{
				if (nullptr == m_stagingBuffer.Get())
					CreateStagingBuffer();
				else
				{
					D3D11_BUFFER_DESC pDesc = {};
					m_stagingBuffer->GetDesc(&pDesc);

					//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
					if (pDesc.ByteWidth < GetBufferDescRef().ByteWidth)
						CreateStagingBuffer();
				}

				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(m_stagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(m_elementStride * _uCount));
					pContext->Unmap(m_stagingBuffer.Get(), 0);
					pContext->CopyResource(GetBufferRef().Get(), m_stagingBuffer.Get());
				}
				break;
			}

		}

		}
	}

	void StructBuffer::GetData(void* _pDest, size_t _byteSize)
	{
		if (m_capacity == 0) {
			return;
		}

		auto pContext = RenderManager::GetInst().Context();

		switch (m_desc.eSBufferType)
		{
			memset(_pDest, 0, _byteSize);
			return;
			//READ_ONLY일 경우에는 데이터 가져오기 불가능.
		case eStructBufferType::READ_ONLY:
		{
			//#ifdef _DEBUG
			//		D3D11_MAPPED_SUBRESOURCE Data = {};
			//
			//		pContext->Map(GetBufferRef().Get(), 0, D3D11_MAP_READ, 0, &Data);
			//
			//		size_t bytesize = m_elementStride * m_elementCount;
			//
			//		memcpy_s(_pDest, _byteSize, Data.pData, bytesize);
			//
			//		pContext->Unmap(GetBufferRef().Get(), 0);
			//#endif
			break;
		}

		//읽기/쓰기 모두 가능한 Struct 버퍼일 경우에는 Staging 버퍼를 통해서 가져온다.
		case eStructBufferType::READ_WRITE:
		{
			if (nullptr == m_stagingBuffer.Get())
				CreateStagingBuffer();
			else
			{
				D3D11_BUFFER_DESC pDesc = {};
				m_stagingBuffer->GetDesc(&pDesc);

				//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
				if (pDesc.ByteWidth < GetBufferDescRef().ByteWidth)
					CreateStagingBuffer();
			}

			pContext->CopyResource(m_stagingBuffer.Get(), GetBufferRef().Get());

			D3D11_MAPPED_SUBRESOURCE Data = {};
			pContext->Map(m_stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

			size_t bytesize = m_elementStride * m_capacity;
			memcpy_s(_pDest, _byteSize, Data.pData, bytesize);

			pContext->Unmap(m_stagingBuffer.Get(), 0);
			break;
		}

		default: break;
		}
	}

	void StructBuffer::BindDataSRV(int _SRVSlot, eShaderStageFlag_ _stageFlag)
	{
		UnbindData();

		m_curBoundView = eBufferViewType::SRV;

		if (0 > _SRVSlot)
		{
			_SRVSlot = (int)m_desc.REGISLOT_t_SRV;
		}

		m_curBoundRegister = _SRVSlot;

		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = m_desc.TargetStageSRV;
		}

		auto pContext = RenderManager::GetInst().Context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}

		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}

		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}

		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}

		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}

		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetShaderResources(_SRVSlot, 1, m_SRV.GetAddressOf());
		}
	}

	void StructBuffer::BindDataUAV(int _UAVSlot)
	{
		//읽기 쓰기 다 가능한 상태가 아닐경우 assert
		ASSERT(eStructBufferType::READ_WRITE == m_desc.eSBufferType, "Unordered Access View는 읽기/쓰기가 모두 가능해야 합니다.");

		UnbindData();

		m_curBoundView = eBufferViewType::UAV;

		if (0 > _UAVSlot)
		{
			_UAVSlot = m_desc.REGISLOT_u_UAV;
		}
		m_curBoundRegister = _UAVSlot;

		uint Offset = -1;
		RenderManager::GetInst().Context()->CSSetUnorderedAccessViews(_UAVSlot, 1, m_UAV.GetAddressOf(), &Offset);
	}

	bool StructBuffer::CreateStagingBuffer()
	{
		//SysMem <-> GPUMem 데이터 전송용 스테이징 버퍼 생성
		D3D11_BUFFER_DESC Desc = GetBufferDescRef();
		Desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		Desc.Usage = D3D11_USAGE_STAGING;
		Desc.BindFlags = 0;
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		bool bResult = SUCCEEDED(RenderManager::GetInst().Device()->CreateBuffer(&Desc, nullptr, m_stagingBuffer.ReleaseAndGetAddressOf()));

		if(false == bResult)
		{
			ERROR_MESSAGE("구조화 버퍼의 데이터를 주고받을 스테이징 버퍼 생성에 실패했습니다.");
		}
			
		return bResult;
	}

	bool StructBuffer::CreateSRV()
	{
		//SRV 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.NumElements = m_capacity;

		bool bResult = SUCCEEDED(RenderManager::GetInst().Device()->CreateShaderResourceView(GetBufferRef().Get(), &SRVDesc, m_SRV.ReleaseAndGetAddressOf()));
		
		if (false == bResult)
		{
			ERROR_MESSAGE("구조화 버퍼의 Shader Resource View 생성에 실패했습니다.");

		}
		return bResult;
	}

	bool StructBuffer::CreateUAV()
	{
		//GPU에서 읽기 및 쓰기 작업이 가능해야 하므로 UAV 형태로 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_capacity;

		bool bResult = SUCCEEDED(RenderManager::GetInst().Device()->CreateUnorderedAccessView(GetBufferRef().Get(), &UAVDesc, m_UAV.ReleaseAndGetAddressOf()));
		if (false == bResult)
		{
			ERROR_MESSAGE("구조화 버퍼의 Unordered Access View 생성에 실패했습니다.");

		}
		return bResult;
	}

	void StructBuffer::UnbindData()
	{
		switch (m_curBoundView)
		{
		case ehw::eBufferViewType::NONE:
			break;
		case ehw::eBufferViewType::SRV:
		{
			auto pContext = RenderManager::GetInst().Context();

			ID3D11ShaderResourceView* pView = nullptr;
			if (eShaderStageFlag::Vertex & m_desc.TargetStageSRV)
			{
				pContext->VSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::Hull & m_desc.TargetStageSRV)
			{
				pContext->HSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::Domain & m_desc.TargetStageSRV)
			{
				pContext->DSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::Geometry & m_desc.TargetStageSRV)
			{
				pContext->GSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::Pixel & m_desc.TargetStageSRV)
			{
				pContext->PSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			if (eShaderStageFlag::Compute & m_desc.TargetStageSRV)
			{
				pContext->CSSetShaderResources(m_curBoundRegister, 1, &pView);
			}

			break;
		}

		case ehw::eBufferViewType::UAV:
		{
			static const uint v2_Offset = -1;
			ID3D11UnorderedAccessView* pUAV = nullptr;
			RenderManager::GetInst().Context()->CSSetUnorderedAccessViews(m_curBoundRegister, 1, &pUAV, &v2_Offset);
			break;
		}

		default:
			ERROR_MESSAGE("알 수 없는 오류");
			break;
		}


		m_curBoundRegister = -1;
		m_curBoundView = eBufferViewType::NONE;
	}

}
