
#include "Texture.h"



#include <Engine/Util/define_Util.h>
#include <Engine/Util/StringConverter.h>

#include <Engine/Manager/ResourceManager.h>


namespace core
{
	Texture::Texture()
		: Resource()
		, m_texture2D_desc()
		, m_texture()
		, m_image()
		, m_SRV()
		, m_UAV()

		, m_DSV()
		, m_RTV()

		, m_cur_bound_view()
		, m_cur_bound_register(-1)
		, m_cur_bound_stage(eShaderStageFlag::NONE)
	{
	}

	Texture::~Texture()
	{

	}

	void Texture::clear_SRV(UINT _startSlot)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		auto pContext = RenderManager::get_inst().get_context();

		pContext->VSSetShaderResources(_startSlot, 1u, &srv);
		pContext->HSSetShaderResources(_startSlot, 1u, &srv);
		pContext->DSSetShaderResources(_startSlot, 1u, &srv);
		pContext->GSSetShaderResources(_startSlot, 1u, &srv);
		pContext->PSSetShaderResources(_startSlot, 1u, &srv);
		pContext->CSSetShaderResources(_startSlot, 1u, &srv);
	}

	void Texture::clear_ALL()
	{
		for (int i = 0; i < (int)eTextureSlot::END; ++i)
		{
			ID3D11ShaderResourceView* srv = nullptr;

			auto pContext = RenderManager::get_inst().get_context();
			pContext->VSSetShaderResources(i, 1u, &srv);
			pContext->HSSetShaderResources(i, 1u, &srv);
			pContext->DSSetShaderResources(i, 1u, &srv);
			pContext->GSSetShaderResources(i, 1u, &srv);
			pContext->PSSetShaderResources(i, 1u, &srv);
			pContext->CSSetShaderResources(i, 1u, &srv);
		}
	}

	bool Texture::create(UINT _width, UINT _height, DXGI_FORMAT _pixelFormat, uint _D3D11_BIND_FLAG, bool _bAllowCPURead)
	{
		m_texture2D_desc.BindFlags = _D3D11_BIND_FLAG;

		////CPU의 읽기/쓰기 가능 여부를 설정
		//텍스처는 CPU가 데이터를 쓸 일이 없다.
		//포스트프로세싱, 컴퓨트쉐이더 작업의 경우는 
		// GPU에서 쓰기 권한 허용(UAV)만 해주면 되는것임
		if (_bAllowCPURead)
		{
			m_texture2D_desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
			m_texture2D_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		}
		else
		{
			m_texture2D_desc.CPUAccessFlags = 0u;
			m_texture2D_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		}

		m_texture2D_desc.Format = _pixelFormat;
		m_texture2D_desc.Width = _width;
		m_texture2D_desc.Height = _height;
		m_texture2D_desc.ArraySize = 1;
		m_texture2D_desc.MiscFlags = 0;

		//원본만 생성.(자세한 내용은 밉맵을 찾아볼 것)
		m_texture2D_desc.MipLevels = 1;
		m_texture2D_desc.SampleDesc.Count = 1;
		m_texture2D_desc.SampleDesc.Quality = 0;
		

		bool bResult = false;
		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateTexture2D(&m_texture2D_desc, nullptr, m_texture.GetAddressOf());

		if(FAILED(result))
		{
			ERROR_MESSAGE("텍스처 생성에 실패 했습니다.");
			m_texture2D_desc = {};
			return false;
		}
		
		bResult = create_view();
		if (false == bResult)
		{
			m_texture = nullptr;
			m_texture2D_desc = {};
		}

		return bResult;
	}

	bool Texture::create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture)
	{
		m_texture = _texture;
		m_texture->GetDesc(&m_texture2D_desc);

		bool Result = create_view();
		
		if (false == Result)
		{
			m_texture = nullptr;
			m_texture2D_desc = {};
		}

		return Result;
	}

	bool Texture::create(const D3D11_TEXTURE2D_DESC& _TexDesc)
	{
		bool Result = false;
		m_texture2D_desc = _TexDesc;

		Result = SUCCEEDED(RenderManager::get_inst().get_device()->CreateTexture2D(&m_texture2D_desc, nullptr, m_texture.GetAddressOf()));
		if (false == Result)
		{
			ERROR_MESSAGE("텍스처 생성에 실패 했습니다.");
			m_texture2D_desc = {};
			return false;
		}

		Result = create_view();
		if(false == Result)
		{
			m_texture2D_desc = {};
			Result = false;
		}
		
		return Result;
	}

	eResult Texture::save(const std::fs::path& _base_directory) const
	{
		return save_file(_base_directory / get_res_filename());
	}

	eResult Texture::load(const std::fs::path& _base_directory)
	{
		std::fs::path fullPath = _base_directory / get_res_filename();

		if (false == std::fs::exists(fullPath))
		{
			std::wstring errMsg = fullPath.wstring() + L"\n";
			errMsg += L"파일이 없습니다.";
			ERROR_MESSAGE_W(errMsg.c_str());
			return eResult::Fail_Open;
		}

		eResult result = load_file(fullPath);

		if (eResult::Success == result)
		{
			initialize_resource();
		}
		return result;
	}

	eResult Texture::load_file(const std::filesystem::path& _fullPath)
	{
		std::wstring Extension = _fullPath.extension().wstring();
		StringConverter::make_upper_case(Extension);

		if (Extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(_fullPath.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_image)))
				return eResult::Fail_Open;
		}
		else if (Extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(_fullPath.c_str(), DirectX::TGA_FLAGS::TGA_FLAGS_NONE, nullptr, m_image)))
				return eResult::Fail_Open;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(_fullPath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_image)))
				return eResult::Fail_Open;
		}

		return eResult::Success;
	}

	eResult Texture::save_file(const std::filesystem::path& _fullPath) const
	{
		//부모 경로 존재 확인
		{
			std::fs::path checkPath = _fullPath.parent_path();

			if (false == std::fs::exists(checkPath))
			{
				if (false == std::fs::create_directories(checkPath))
				{
					ERROR_MESSAGE("경로가 잘못되었습니다.");
					return eResult::Fail_Open;
				}
			}
		}

		auto device = RenderManager::get_inst().get_device();
		auto context = RenderManager::get_inst().get_context();

		std::wstring Extension = _fullPath.extension().wstring();

		DirectX::ScratchImage img{};
		HRESULT result = DirectX::CaptureTexture(device.Get(), context.Get(), m_texture.Get(), img);

		if (FAILED(result))
		{
			ERROR_MESSAGE("Texture를 Scratch Image로 가져오는 데 실패했습니다.");
			return eResult::Fail_Create;
		}

		StringConverter::make_upper_case(Extension);
		if (Extension == L".DDS")
		{
			if (FAILED(DirectX::SaveToDDSFile(img.GetImages(), img.GetImageCount(), img.GetMetadata(), DirectX::DDS_FLAGS_NONE, _fullPath.wstring().c_str())))
			{
				return eResult::Fail_Create;
			}
		}
		else if (Extension == L".TGA")
		{
			if (FAILED(DirectX::SaveToTGAFile(*(img.GetImage(0, 0, 0)), DirectX::TGA_FLAGS::TGA_FLAGS_NONE, _fullPath.wstring().c_str(), &img.GetMetadata())))
			{
				return eResult::Fail_Create;
			}
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(DirectX::SaveToWICFile(img.GetImages(), img.GetImageCount(), DirectX::WIC_FLAGS_NONE, DirectX::GetWICCodec(DirectX::WICCodecs::WIC_CODEC_PNG), _fullPath.wstring().c_str())))
			{
				return eResult::Fail_Create;
			}
		}

		return eResult::Success;
	}

	void Texture::initialize_resource()
	{
		CreateShaderResourceView
		(
			RenderManager::get_inst().get_device().Get(),
			m_image.GetImages(),
			m_image.GetImageCount(),
			m_image.GetMetadata(),
			m_SRV.GetAddressOf()
		);

		m_SRV->GetResource((ID3D11Resource**)m_texture.GetAddressOf());
		m_texture->GetDesc(&m_texture2D_desc);
	}

	void Texture::bind_buffer_as_SRV(uint _SRVSlot, eShaderStageFlag_ _stageFlag)
	{
		unbind_buffer_from_GPU_register();
	
		m_cur_bound_register = (int)_SRVSlot;
		m_cur_bound_stage = _stageFlag;
		m_cur_bound_view = eBufferViewType::SRV;

		auto pContext = RenderManager::get_inst().get_context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetShaderResources(_SRVSlot, 1u, m_SRV.GetAddressOf());
		}
	}

	void Texture::bind_buffer_as_UAV(uint _startSlot)
	{
		unbind_buffer_from_GPU_register();

		m_cur_bound_view = eBufferViewType::UAV;
		m_cur_bound_register = (int)_startSlot;

		uint i = -1;
		RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(_startSlot, 1, m_UAV.GetAddressOf(), &i);
	}


	void Texture::unbind_buffer_from_GPU_register()
	{
		switch (m_cur_bound_view)
		{
		case core::eBufferViewType::NONE:
			break;

		case core::eBufferViewType::SRV:
		{
			ASSERT(0 <= m_cur_bound_register, "Bound된 레지스터 번호가 음수입니다.");
			ID3D11ShaderResourceView* srv = nullptr;

			auto pContext = RenderManager::get_inst().get_context();

			if (eShaderStageFlag::Vertex & m_cur_bound_stage)
			{
				pContext->VSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}
			if (eShaderStageFlag::Hull & m_cur_bound_stage)
			{
				pContext->HSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}
			if (eShaderStageFlag::Domain & m_cur_bound_stage)
			{
				pContext->DSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}
			if (eShaderStageFlag::Geometry & m_cur_bound_stage)
			{
				pContext->GSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}
			if (eShaderStageFlag::Pixel & m_cur_bound_stage)
			{
				pContext->PSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}
			if (eShaderStageFlag::Compute & m_cur_bound_stage)
			{
				pContext->CSSetShaderResources(m_cur_bound_register, 1u, &srv);
			}

			m_cur_bound_register = -1;
			m_cur_bound_stage = eShaderStageFlag::NONE;
			
			break;
		}
		case core::eBufferViewType::UAV:
		{
			ID3D11UnorderedAccessView* pUAV = nullptr;
			uint u = -1;

			RenderManager::get_inst().get_context()->CSSetUnorderedAccessViews(m_cur_bound_register, 1, &pUAV, &u);

			//현재 연결된 레지스터 번호와 파이프라인을 초기화
			m_cur_bound_register = -1;
			m_cur_bound_stage = eShaderStageFlag::NONE;

			break;
		}
			
		case core::eBufferViewType::RTV:
			[[fallthrough]];
		case core::eBufferViewType::DSV:
		{
			ID3D11RenderTargetView* pRTV = nullptr;
			ID3D11DepthStencilView* pDSV = nullptr;

			RenderManager::get_inst().get_context()->OMSetRenderTargets(1u, &pRTV, pDSV);
			break;
		}

		default:
			break;
		}


		m_cur_bound_view = eBufferViewType::NONE;
	}

	bool Texture::create_view()
	{
		auto pDevice = RenderManager::get_inst().get_device();
		if (m_texture2D_desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(pDevice->CreateDepthStencilView(m_texture.Get(), nullptr, m_DSV.GetAddressOf())))
			{
				ERROR_MESSAGE("Depth Stencil View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (m_texture2D_desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(pDevice->CreateRenderTargetView(m_texture.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				ERROR_MESSAGE("Render Target View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (m_texture2D_desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = m_texture2D_desc.Format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			if (FAILED(pDevice->CreateShaderResourceView(m_texture.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				ERROR_MESSAGE("Shader Resource View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (m_texture2D_desc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = m_texture2D_desc.Format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (FAILED(pDevice->CreateUnorderedAccessView(m_texture.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				ERROR_MESSAGE("Unordered Access View 생성에 실패 했습니다.");
				return false;
			}
		}

		return true;
	}


}
