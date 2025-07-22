#include "StructBuffer.h"

#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/GPU/ConstBuffer.h>

namespace core
{
	StructBuffer::StructBuffer()
		: GPUBuffer(eBufferType::Struct)
		, m_struct_buffer_desc()
		, m_data_stride()
		, m_size()
		, m_capacity()
		, m_SRV()
		, m_UAV()
		, m_staging_buffer()
		, m_current_bound_view()
		, m_current_bound_register(-1)
	{
	}

	StructBuffer::StructBuffer(const StructBuffer& _other)
		: GPUBuffer(_other)
		, m_struct_buffer_desc()
		, m_data_stride(_other.m_data_stride)
		, m_size()
		, m_capacity()
		, m_SRV()
		, m_UAV()
		, m_staging_buffer()
		, m_current_bound_view()
		, m_current_bound_register(-1)
	{
		//set_desc 함수를 통해 설정
		set_desc(_other.m_struct_buffer_desc);

		//버퍼 빈 상태 및 동일 조건으로 생성
		resize(_other.m_capacity, nullptr, 0u);

		//리소스 내용 복사
		ComPtr<ID3D11Buffer> buf = get_buffer();
		RenderManager::get_inst().get_context()->CopyResource(buf.Get(), _other.get_buffer().Get());
	}

	StructBuffer::~StructBuffer()
	{
		unbind_buffer();
	}

	void StructBuffer::set_desc(const tDesc& _desc)
	{
		m_struct_buffer_desc = _desc;

		D3D11_BUFFER_DESC buf_desc = get_buffer_desc();

		switch (m_struct_buffer_desc.m_buffer_RW_type)
		{
		case eStructBufferType::READ_ONLY:

			//CPU에서 구조화버퍼로 작성할 수 있어야 하므로 AccessFlag를 write로 설정
			buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
			buf_desc.Usage = D3D11_USAGE_DYNAMIC;

			//SRV만 바인딩되도록 설정
			buf_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			break;
		case eStructBufferType::READ_WRITE:

			//CPU는 직접 접근 불가, 
			buf_desc.CPUAccessFlags = 0;

			//GPU의 읽기/쓰기는 가능
			buf_desc.Usage = D3D11_USAGE_DEFAULT;

			//SRV, UAV 둘 다 바인딩되도록 설정
			buf_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

			//READ_WRITE로 사용하겠다는 건 컴퓨트쉐이더를 사용하겠다는 의미 -> 실수 방지를 위해 플래그에 컴퓨트쉐이더 추가
			m_struct_buffer_desc.m_SRV_target_stage |= eShaderStageFlag::Compute;

			break;
		default:
			break;
		}

		//기타 플래그에 구조화 버퍼로 설정값이 있음.
		buf_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		set_buffer_desc(buf_desc);
	}

	eResult StructBuffer::resize(size_t _capacity, const void* _pInitialData, UINT _uElemCount)
	{
		if (_capacity == 0) {
			return eResult::Success;
		}
		//이미 더 큰 capacity일 경우 그냥 그 버퍼를 사용한다
		if (_capacity <= m_capacity) {
			set_data(_pInitialData, _uElemCount);
			return eResult::Success;
		}
		else if (_capacity < _uElemCount)
		{
			ERROR_MESSAGE_A("Element capacity of structured buffer must be more than element of input data!");
			return eResult::Fail_Create;
		}

		//재할당 하기 전 바인딩된 리소스가 있다면 unbind
		unbind_buffer();

		//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
		m_size = (UINT)_uElemCount;
		m_capacity = (UINT)_capacity;

		D3D11_BUFFER_DESC buf_desc = get_buffer_desc();
		buf_desc.StructureByteStride = m_data_stride;
		buf_desc.ByteWidth = m_data_stride * m_capacity;

		switch (m_struct_buffer_desc.m_buffer_RW_type)
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
				Data.SysMemPitch = m_data_stride * (UINT)_uElemCount;
				Data.SysMemSlicePitch = m_data_stride;
				//Data.SysMemSlicePitch = m_buffer_desc.StructureByteStride;
				pData = &Data;
			}

			ComPtr<ID3D11Buffer> buf = nullptr;
			//구조화버퍼 생성. ReleaseAndGetAddressOf() 함수를 통해서 기존 구조화 버퍼가 있다면 날려버리고 생성
			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&buf_desc, pData, buf.ReleaseAndGetAddressOf());

			if (FAILED(result))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return eResult::Fail_Create;
			}

			set_buffer(buf);
			set_buffer_desc(buf_desc);

			//Read Only의 경우 바로 SRV 생성
			create_SRV();

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
				Data.SysMemPitch = m_data_stride * (UINT)_uElemCount;

				//이건 사용되지 않음.(3D Texture에서만 사용된다고 함)
				//Data.SysMemSlicePitch = m_buffer_desc.StructureByteStride;
				pData = &Data;
			}

			ComPtr<ID3D11Buffer> buf = nullptr;

			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&buf_desc, pData, buf.ReleaseAndGetAddressOf());
			//구조화버퍼 생성
			if (FAILED(result))
			{
				ERROR_MESSAGE_A("Failed to create Structured Buffer!");
				return eResult::Fail_Create;
			}

			set_buffer(buf);
			set_buffer_desc(buf_desc);

			//SRV 생성
			create_SRV();
			create_UAV();

			break;
		}

		}

		return eResult::Success;
	}

	void StructBuffer::set_data(const void* _pData, UINT _uCount)
	{
		if (nullptr == _pData || 0 == _uCount) { return; }
		m_size = _uCount;

		//생성 시 할당된 갯수보다 들어온 갯수가 더 클 경우 재할당하고, 거기에 데이터를 추가.
		//생성될 때 값을 지정할 수 있으므로 바로 return 해주면 될듯
		if (_uCount > m_capacity)
		{
			//다시 생성하고자 할때는 초기 데이터와 사이즈를 일치시켜서 생성해줘야 한다.
			resize(_uCount, _pData, _uCount);
			return;
		}

		auto pContext = RenderManager::get_inst().get_context();
		switch (m_struct_buffer_desc.m_buffer_RW_type)
		{
		case eStructBufferType::READ_ONLY:
		{
			{
				//Read Only일 경우 SBuffer에 바로 Map/UnMap을 해주면 된다.
				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(get_buffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(m_data_stride * _uCount));

					pContext->Unmap(get_buffer().Get(), 0);
				}

				break;
			}
		}

		case eStructBufferType::READ_WRITE:
		{
			{
				if (m_staging_buffer.Get()) 
				{
					D3D11_BUFFER_DESC pDesc = {};
					m_staging_buffer->GetDesc(&pDesc);

					//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
					if (pDesc.ByteWidth < get_buffer_desc().ByteWidth)
					{
						create_staging_buffer();
					}
				}
				else
				{
					bool result = create_staging_buffer();
					if (false == result)
					{
						return;
					}
				}

				D3D11_MAPPED_SUBRESOURCE Data = {};
				if (SUCCEEDED(pContext->Map(m_staging_buffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data)))
				{
					memcpy(Data.pData, _pData, (size_t)(m_data_stride * _uCount));
					pContext->Unmap(m_staging_buffer.Get(), 0);
					pContext->CopyResource(get_buffer().Get(), m_staging_buffer.Get());
				}
				break;
			}

		}

		}
	}

	void StructBuffer::get_data(void* _pDest, size_t _byteSize)
	{
		if (m_capacity == 0) {
			return;
		}

		auto pContext = RenderManager::get_inst().get_context();

		switch (m_struct_buffer_desc.m_buffer_RW_type)
		{
			memset(_pDest, 0, _byteSize);
			return;
			//READ_ONLY일 경우에는 데이터 가져오기 불가능.
		case eStructBufferType::READ_ONLY:
		{
			ERROR_MESSAGE("CPU ACCESS FLAG가 Read Only일 경우 데이터를 읽어올 수 없습니다!");
			//#ifdef _DEBUG
			//		D3D11_MAPPED_SUBRESOURCE Data = {};
			//
			//		pContext->Map(get_buffer().Get(), 0, D3D11_MAP_READ, 0, &Data);
			//
			//		size_t bytesize = m_data_stride * m_size;
			//
			//		memcpy_s(_pDest, _byteSize, Data.pData, bytesize);
			//
			//		pContext->Unmap(get_buffer().Get(), 0);
			//#endif
			break;
		}

		//읽기/쓰기 모두 가능한 Struct 버퍼일 경우에는 Staging 버퍼를 통해서 가져온다.
		case eStructBufferType::READ_WRITE:
		{
			if (nullptr == m_staging_buffer.Get())
			{
				create_staging_buffer();
			}
			else
			{
				D3D11_BUFFER_DESC pDesc = {};
				m_staging_buffer->GetDesc(&pDesc);

				//생성되어 있는 Staging Buffer의 크기보다 클 경우 새로 생성
				if (pDesc.ByteWidth < get_buffer_desc().ByteWidth)
				{
					create_staging_buffer();
				}
			}

			pContext->CopyResource(m_staging_buffer.Get(), get_buffer().Get());

			D3D11_MAPPED_SUBRESOURCE Data = {};
			pContext->Map(m_staging_buffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

			size_t bytesize = m_data_stride * m_capacity;
			memcpy_s(_pDest, _byteSize, Data.pData, bytesize);

			pContext->Unmap(m_staging_buffer.Get(), 0);
			break;
		}

		default: break;
		}
	}

	void StructBuffer::bind_buffer_as_SRV(int32 _SRVSlot, eShaderStageFlag_ _stageFlag)
	{
		unbind_buffer();

		m_current_bound_view = eBufferViewType::SRV;

		if (0 > _SRVSlot)
		{
			_SRVSlot = (int32)m_struct_buffer_desc.m_SRV_target_register_idx;
		}

		m_current_bound_register = _SRVSlot;

		if (eShaderStageFlag::NONE == _stageFlag)
		{
			_stageFlag = m_struct_buffer_desc.m_SRV_target_stage;
		}

		auto pContext = RenderManager::get_inst().get_context();
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

	void StructBuffer::bind_buffer_as_UAV(int32 _UAVSlot)
	{
		//읽기 쓰기 다 가능한 상태가 아닐경우 assert
		ASSERT(eStructBufferType::READ_WRITE == m_struct_buffer_desc.m_buffer_RW_type, "Unordered Access View는 읽기/쓰기가 모두 가능해야 합니다.");

		unbind_buffer();

		m_current_bound_view = eBufferViewType::UAV;

		if (0 > _UAVSlot)
		{
			_UAVSlot = m_struct_buffer_desc.m_UAV_target_register_idx;
		}
		m_current_bound_register = _UAVSlot;

		UINT Offset = -1;
		RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(_UAVSlot, 1, m_UAV.GetAddressOf(), &Offset);
	}

	bool StructBuffer::create_staging_buffer()
	{
		//SysMem <-> GPUMem 데이터 전송용 스테이징 버퍼 생성
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = get_buffer_desc().ByteWidth;
		desc.StructureByteStride = get_buffer_desc().StructureByteStride;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

		HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, nullptr, m_staging_buffer.ReleaseAndGetAddressOf());

		if(FAILED(result))
		{
			ERROR_MESSAGE("구조화 버퍼의 데이터를 주고받을 스테이징 버퍼 생성에 실패했습니다.");
			return false;
		}
			
		return true;
	}

	bool StructBuffer::create_SRV()
	{
		//SRV 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.NumElements = m_capacity;
		
		HRESULT result = RenderManager::get_inst().get_device()->CreateShaderResourceView(get_buffer().Get(), &SRVDesc, m_SRV.ReleaseAndGetAddressOf());
		
		if (FAILED(result))
		{
			ERROR_MESSAGE("구조화 버퍼의 Shader Resource View 생성에 실패했습니다.");
			return false;
		}
		return true;
	}

	bool StructBuffer::create_UAV()
	{
		//GPU에서 읽기 및 쓰기 작업이 가능해야 하므로 UAV 형태로 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_capacity;

		bool bResult = SUCCEEDED(RenderManager::get_inst().get_device()->CreateUnorderedAccessView(get_buffer().Get(), &UAVDesc, m_UAV.ReleaseAndGetAddressOf()));
		if (false == bResult)
		{
			ERROR_MESSAGE("구조화 버퍼의 Unordered Access View 생성에 실패했습니다.");
		}
		return bResult;
	}

	void StructBuffer::unbind_buffer()
	{
		switch (m_current_bound_view)
		{
		case core::eBufferViewType::NONE:
			break;
		case core::eBufferViewType::SRV:
		{
			auto pContext = RenderManager::get_inst().get_context();

			ID3D11ShaderResourceView* pView = nullptr;
			if (eShaderStageFlag::Vertex & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->VSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			if (eShaderStageFlag::Hull & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->HSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			if (eShaderStageFlag::Domain & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->DSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			if (eShaderStageFlag::Geometry & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->GSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			if (eShaderStageFlag::Pixel & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->PSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			if (eShaderStageFlag::Compute & m_struct_buffer_desc.m_SRV_target_stage)
			{
				pContext->CSSetShaderResources(m_current_bound_register, 1, &pView);
			}

			break;
		}

		case core::eBufferViewType::UAV:
		{
			static const UINT v2_Offset = -1;
			ID3D11UnorderedAccessView* pUAV = nullptr;
			RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(m_current_bound_register, 1, &pUAV, &v2_Offset);
			break;
		}

		default:
			ERROR_MESSAGE("알 수 없는 오류");
			break;
		}


		m_current_bound_register = -1;
		m_current_bound_view = eBufferViewType::NONE;
	}

}
