#include "StructBuffer.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	StructBuffer::StructBuffer()
		: GPUBuffer(eBufferType::Struct)
	{
	}

	StructBuffer::~StructBuffer()
	{
		unbind();
	}

	bool StructBuffer::create(const tDesc& _desc)
	{
		if (m_buffer != nullptr)
		{
			ERROR_MESSAGE("이미 생성된 buffer입니다. 새로운 클래스에 생성하세요.");
			return false;
		}
		if (0 != (m_byte_stride % 16))
		{
			ERROR_MESSAGE("byte stride는 16의 배수여야 합니다.");
			return false;
		}

		m_buffer_desc = _desc.buffer_desc;
		m_access_type = _desc.access_type;
		m_target_SRV_slot = _desc.SRV_slot;
		m_target_UAV_slot = _desc.UAV_slot;
		m_byte_stride = _desc.byte_stride;

		return resize(_desc.capacity, _desc.p_initial_data, _desc.element_count);
	}

	bool StructBuffer::resize(UINT _new_capacity, const void* _initial_data, UINT _elem_count)
	{
		if (0 == m_byte_stride)
		{
			ERROR_MESSAGE("byte stride가 0입니다.");
			return false;
		}

		//capacity 0일시 만들 필요가 없음
		if (_new_capacity == 0) {
			return true;
		}
		//이미 더 큰 capacity일 경우 그냥 그 버퍼를 사용한다
		if (_new_capacity <= m_capacity) {
			set_data(_initial_data, _elem_count);
			return true;
		}
		if (_new_capacity < _elem_count)
		{
			ERROR_MESSAGE("데이터 사이즈보다 capacity를 작게 설정했습니다.");
		}

		//재할당 하기 전 바인딩된 리소스가 있다면 unbind
		unbind();

		//상수버퍼와는 다르게 버퍼 재할당이 가능함. 먼저 기존 버퍼의 할당을 해제한다.(ComPtr을 통해 관리가 이루어지므로 nullptr로 바꿔주면 됨.)
		ComPtr<ID3D11Buffer> buf = nullptr;
		D3D11_BUFFER_DESC desc = m_buffer_desc;

		ComPtr<ID3D11ShaderResourceView> SRV = nullptr;
		ComPtr<ID3D11UnorderedAccessView> UAV = nullptr;

		desc.StructureByteStride = m_byte_stride;
		desc.ByteWidth = m_byte_stride * _new_capacity;

		//초깃값 업로드 및 바인딩
		D3D11_SUBRESOURCE_DATA* pData = nullptr;
		D3D11_SUBRESOURCE_DATA Data = {};
		if (nullptr != _initial_data)
		{
			Data.pSysMem = _initial_data;

			//이 값은 무시됨. BufferDesc.ByteWidth 값만 영향을 미치는 것을 확인함.
			Data.SysMemPitch = m_byte_stride * _elem_count;
			Data.SysMemSlicePitch = m_byte_stride;
			pData = &Data;
		}

		switch (m_access_type)
		{
			//일반적인 GPU에서 읽기만 가능한 구조화 버퍼
		case eStructBufferAccessType::READ_ONLY:
		{
			//구조화버퍼 생성. ReleaseAndGetAddressOf() 함수를 통해서 기존 구조화 버퍼가 있다면 날려버리고 생성
			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, pData, buf.ReleaseAndGetAddressOf());

			if (FAILED(result))
			{
				ERROR_MESSAGE("Failed to create Structured Buffer!");
				return false;
			}

			SRV = create_SRV(buf, _new_capacity);
			if (nullptr == SRV)
			{
				ERROR_MESSAGE("SRV 생성 실패.");
				return false;
			}

			break;
		}

		//Compute Shader과 결합해서 사용하는, GPU에서 조작 가능한 SBuffer
		//이 경우에는 CPU와 데이터 전송이 불가능하므로 데이터 전송용 Staging Buffer를 중간 단계로 사용해야 한다.
		case eStructBufferAccessType::READ_WRITE:
		{
			//초깃값 지정
			D3D11_SUBRESOURCE_DATA* pData = nullptr;
			D3D11_SUBRESOURCE_DATA Data = {};
			if (nullptr != _initial_data)
			{
				Data.pSysMem = _initial_data;
				Data.SysMemPitch = m_byte_stride * _elem_count;

				//이건 사용되지 않음.(3D Texture에서만 사용된다고 함)
				//Data.SysMemSlicePitch = m_buffer_desc.StructureByteStride;
				pData = &Data;
			}

			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, pData, buf.ReleaseAndGetAddressOf());
			//구조화버퍼 생성
			if (FAILED(result))
			{
				ERROR_MESSAGE("Failed to create Structured Buffer!");
				return false;
			}

			SRV = create_SRV(buf, _new_capacity);
			if (nullptr == SRV)
			{
				ERROR_MESSAGE("SRV 생성 실패.");
				return false;
			}

			UAV = create_UAV(buf, _new_capacity);
			if (nullptr == UAV)
			{
				ERROR_MESSAGE("UAV 생성 실패.");
				return false;
			}

			break;
		}

		case eStructBufferAccessType::COMPUTE_ONLY:
		{
			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, pData, buf.ReleaseAndGetAddressOf());
			//구조화버퍼 생성
			if (FAILED(result))
			{
				ERROR_MESSAGE("Failed to create Structured Buffer!");
				return false;
			}

			UAV = create_UAV(buf, _new_capacity);
			if (nullptr == UAV)
			{
				ERROR_MESSAGE("UAV 생성 실패.");
				return false;
			}
			break;
		}

		case eStructBufferAccessType::IMMUTABLE:
		{
			HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, pData, buf.ReleaseAndGetAddressOf());
			//구조화버퍼 생성
			if (FAILED(result))
			{
				ERROR_MESSAGE("Failed to create Structured Buffer!");
				return false;
			}

			SRV = create_SRV(buf, _new_capacity);
			if (nullptr == SRV)
			{
				ERROR_MESSAGE("SRV 생성 실패.");
				return false;
			}
			break;
		}
		default:
		{
			ASSERT_RELEASE(false);
		}
		}

		m_buffer = buf;
		m_buffer_desc = desc;
		m_SRV = SRV;
		m_UAV = UAV;

		return true;
	}

	void StructBuffer::set_data(const void* _pData, UINT _uCount)
	{
		if (nullptr == _pData || 0 == _uCount) 
		{ 
			DEBUG_MESSAGE("업로드할 데이터가 없거나 갯수가 0입니다.");
			return; 
		}
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

		if (m_access_type == eStructBufferAccessType::READ_ONLY)
		{
			//Read Only일 경우 SBuffer에 바로 Map/UnMap을 해주면 된다.
			D3D11_MAPPED_SUBRESOURCE Data = {};
			HRESULT result = pContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);
			{
				memcpy(Data.pData, _pData, (size_t)m_byte_stride * (size_t)_uCount);
				pContext->Unmap(m_buffer.Get(), 0);
			}
		}

		else if (m_access_type == eStructBufferAccessType::READ_WRITE
			|| m_access_type == eStructBufferAccessType::READ_WRITE)
		{
			if (m_upload_buffer_byte_width < m_buffer_desc.ByteWidth)
			{
				m_upload_buffer = create_staging_buffer(m_buffer_desc.ByteWidth, false, true);
				m_upload_buffer_byte_width = m_buffer_desc.ByteWidth;

				if (nullptr == m_upload_buffer.Get())
				{
					ERROR_MESSAGE("업로드용 Staging Buffer 생성 실패.");
					return;
				}
			}

			D3D11_MAPPED_SUBRESOURCE Data = {};
			HRESULT result = pContext->Map(m_upload_buffer.Get(), 0, D3D11_MAP_WRITE, 0, &Data);
			if (SUCCEEDED(result))
			{
				memcpy(Data.pData, _pData, (size_t)m_byte_stride * (size_t)_uCount);
				pContext->Unmap(m_upload_buffer.Get(), 0);
				pContext->CopyResource(m_buffer.Get(), m_upload_buffer.Get());
			}
		}
		else if (m_access_type == eStructBufferAccessType::IMMUTABLE)
		{
			ERROR_MESSAGE("IMMUTABLE로 생성된 Struct Buffer는 데이터를 변경할 수 없습니다.");
			return;
		}
		else
		{
			ASSERT(false);
		}
	}

	void StructBuffer::get_data(void* _pDest, size_t _byteSize)
	{
		if (m_capacity == 0) {
			return;
		}

		//데이터를 가져오는 건 '무조건' Staging Buffer를 통해서만 가능하다.
		if (m_readback_buffer_byte_width < m_buffer_desc.ByteWidth)
		{
			m_readback_buffer = create_staging_buffer(m_buffer_desc.ByteWidth, true, false);
			m_readback_buffer_byte_width = m_buffer_desc.ByteWidth;
			if (nullptr == m_readback_buffer.Get())
			{
				ERROR_MESSAGE("데이터 읽기용 Staging Buffer 생성 실패.");
				return;
			}
		}

		auto pContext = RenderManager::get_inst().get_context();

		pContext->CopyResource(m_readback_buffer.Get(), m_buffer.Get());

		D3D11_MAPPED_SUBRESOURCE Data = {};
		pContext->Map(m_readback_buffer.Get(), 0, D3D11_MAP_READ, 0, &Data);

		size_t bytesize = m_byte_stride * m_capacity;
		memcpy_s(_pDest, _byteSize, Data.pData, bytesize);

		pContext->Unmap(m_readback_buffer.Get(), 0);
	}

	void StructBuffer::bind_SRV(eShaderStageFlag_ _stageFlag)
	{
		if (m_current_bound_view == eBufferViewType::UAV)
		{
			unbind();
		}
		m_current_bound_view = eBufferViewType::SRV;

		auto pContext = RenderManager::get_inst().get_context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Vertex][m_target_SRV_slot] = m_buffer;
		}

		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Hull][m_target_SRV_slot] = m_buffer;
		}

		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Domain][m_target_SRV_slot] = m_buffer;
		}

		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Geometry][m_target_SRV_slot] = m_buffer;
		}

		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Pixel][m_target_SRV_slot] = m_buffer;
		}

		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetShaderResources(m_target_SRV_slot, 1, m_SRV.GetAddressOf());
			s_bound_table_SRV[(int)eShaderStageFlag::Compute][m_target_SRV_slot] = m_buffer;
		}
	}

	void StructBuffer::bind_UAV()
	{
		if (m_current_bound_view == eBufferViewType::SRV)
		{
			unbind();
		}
		m_current_bound_view = eBufferViewType::UAV;

		UINT Offset = -1;
		RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(m_target_UAV_slot, 1, m_UAV.GetAddressOf(), &Offset);
	}

	ComPtr<ID3D11Buffer> StructBuffer::create_staging_buffer(UINT _byte_width, bool _enable_read, bool _enable_write)
	{
		ComPtr<ID3D11Buffer> ret = nullptr;

		//SysMem <-> GPUMem 데이터 전송용 스테이징 버퍼 생성
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = _byte_width;
		desc.StructureByteStride = 0;
		desc.MiscFlags = 0;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;

		if (_enable_write)
		{
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
		}
		if (_enable_read)
		{
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
		}

		HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&desc, nullptr, ret.ReleaseAndGetAddressOf());

		if(FAILED(result))
		{
			ERROR_MESSAGE("구조화 버퍼의 데이터를 주고받을 스테이징 버퍼 생성에 실패했습니다.");
			return ret;
		}
			
		return ret;
	}

	ComPtr<ID3D11ShaderResourceView> StructBuffer::create_SRV(const ComPtr<ID3D11Buffer>& _buf, UINT _capacity)
	{
		ComPtr<ID3D11ShaderResourceView> ret = nullptr;

		//SRV 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;
		SRVDesc.BufferEx.NumElements = _capacity;
		
		HRESULT result = RenderManager::get_inst().get_device()->CreateShaderResourceView(_buf.Get(), &SRVDesc, ret.GetAddressOf());
		
		if (FAILED(result))
		{
			ERROR_MESSAGE("구조화 버퍼의 Shader Resource View 생성에 실패했습니다.");
			return nullptr;
		}

		return ret;
	}

	ComPtr<ID3D11UnorderedAccessView> StructBuffer::create_UAV(const ComPtr<ID3D11Buffer>& _buf, UINT _capacity)
	{
		ComPtr<ID3D11UnorderedAccessView> ret = nullptr;
		//GPU에서 읽기 및 쓰기 작업이 가능해야 하므로 UAV 형태로 생성
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		UAVDesc.Buffer.NumElements = m_capacity;

		HRESULT result = RenderManager::get_inst().get_device()->CreateUnorderedAccessView(_buf.Get(), &UAVDesc, ret.GetAddressOf());
		if (FAILED(result))
		{
			ERROR_MESSAGE("구조화 버퍼의 Unordered Access View 생성에 실패했습니다.");
			return nullptr;
		}

		return ret;
	}

	eResult StructBuffer::save(const std::fs::path& _fullpath) const
	{
		return eResult::Fail;
	}

	eResult StructBuffer::load(const std::fs::path& _fullpath)
	{
		return eResult::Fail;
	}

	void StructBuffer::unbind()
	{
		switch (m_current_bound_view)
		{
		case core::eBufferViewType::NONE: return;
		case core::eBufferViewType::SRV:
		{
			auto pContext = RenderManager::get_inst().get_context();

			for (int i = 0; i < (int)eShaderStage::END; ++i)
			{
				if(s_bound_table_SRV[i][m_target_SRV_slot] == m_buffer)
				{
					s_bound_table_SRV[i][m_target_SRV_slot] = nullptr;
					switch (static_cast<eShaderStage>(i))
					{
					case eShaderStage::Vertex:
						pContext->VSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					case eShaderStage::Hull:
						pContext->HSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					case eShaderStage::Domain:
						pContext->DSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					case eShaderStage::Geometry:
						pContext->GSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					case eShaderStage::Pixel:
						pContext->PSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					case eShaderStage::Compute:
						pContext->CSSetShaderResources(m_target_SRV_slot, 1, nullptr);
						break;
					default:
						break;
					}
				}
			}
			break;
		}

		case core::eBufferViewType::UAV:
		{
			static const UINT v2_Offset = -1;
			ID3D11UnorderedAccessView* pUAV = nullptr;
			RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(m_target_SRV_slot, 1, &pUAV, &v2_Offset);
			break;
		}

		default:
			ERROR_MESSAGE("알 수 없는 오류");
			break;
		}

		m_current_bound_view = eBufferViewType::NONE;
	}

	StructBuffer::tDesc::tDesc(UINT _byte_stride, eStructBufferAccessType _access_type)
	{
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		switch (_access_type)
		{
		case core::eStructBufferAccessType::NOT_SET:
			ERROR_MESSAGE("구조화 버퍼의 접근 타입이 설정되지 않았습니다.");
			break;
		case core::eStructBufferAccessType::READ_ONLY:
			//CPU에서 구조화버퍼로 작성할 수 있어야 하므로 AccessFlag를 write로 설정
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			//CPU에서 버퍼를 작성하고 GPU에서 버퍼를 읽어들일 경우 사용
			buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			//SRV만 바인딩되도록 설정
			buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			break;
		case core::eStructBufferAccessType::READ_WRITE:
			//CPU는 직접 접근 불가, 
			buffer_desc.CPUAccessFlags = 0;
			//GPU의 읽기/쓰기는 가능
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			//SRV, UAV 둘 다 바인딩되도록 설정
			buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			break;
		case core::eStructBufferAccessType::COMPUTE_ONLY:
			//CPU는 직접 접근 불가, 
			buffer_desc.CPUAccessFlags = 0;
			//GPU의 읽기/쓰기는 가능
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
			break;

		case core::eStructBufferAccessType::IMMUTABLE:
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			break;
		default:
			break;
		}
	}

}
