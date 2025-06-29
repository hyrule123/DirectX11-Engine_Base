#include "Engine/Resource/Texture.h"



#include "Engine/Util/define_Util.h"
#include "Engine/Util/StringConverter.h"

#include "Engine/Manager/ResourceManager.h"


namespace core
{
	Texture::Texture()
		: Resource(Texture::s_static_type_name)
		, mDesc()
		, mTexture()
		, mImage()
		, mSRV()
		, mUAV()

		, mDSV()
		, mRTV()

		, mCurBoundView()
		, mCurBoundRegister(-1)
		, mCurBoundStage(eShaderStageFlag::NONE)
	{
	}

	Texture::~Texture()
	{

	}

	void Texture::ClearSRV(UINT _startSlot)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		auto pContext = RenderManager::get_inst().Context();

		pContext->VSSetShaderResources(_startSlot, 1u, &srv);
		pContext->HSSetShaderResources(_startSlot, 1u, &srv);
		pContext->DSSetShaderResources(_startSlot, 1u, &srv);
		pContext->GSSetShaderResources(_startSlot, 1u, &srv);
		pContext->PSSetShaderResources(_startSlot, 1u, &srv);
		pContext->CSSetShaderResources(_startSlot, 1u, &srv);
	}

	void Texture::ClearAll()
	{
		for (int i = 0; i < (int)eTextureSlot::END; ++i)
		{
			ID3D11ShaderResourceView* srv = nullptr;

			auto pContext = RenderManager::get_inst().Context();
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
		mDesc.BindFlags = _D3D11_BIND_FLAG;

		////CPU의 읽기/쓰기 가능 여부를 설정
		//텍스처는 CPU가 데이터를 쓸 일이 없다.
		//포스트프로세싱, 컴퓨트쉐이더 작업의 경우는 
		// GPU에서 쓰기 권한 허용(UAV)만 해주면 되는것임
		if (_bAllowCPURead)
		{
			mDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
			mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		}
		else
		{
			mDesc.CPUAccessFlags = 0u;
			mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		}

		mDesc.Format = _pixelFormat;
		mDesc.Width = _width;
		mDesc.Height = _height;
		mDesc.ArraySize = 1;
		mDesc.MiscFlags = 0;

		//원본만 생성.(자세한 내용은 밉맵을 찾아볼 것)
		mDesc.MipLevels = 1;
		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;
		

		bool bResult = false;
		auto* pDevice = RenderManager::get_inst().Device();
		bResult = SUCCEEDED(pDevice->CreateTexture2D(&mDesc, nullptr, mTexture.GetAddressOf()));

		if(false == bResult)
		{
			ERROR_MESSAGE("텍스처 생성에 실패 했습니다.");
			mDesc = {};
			return false;
		}
		
		bResult = CreateView();
		if (false == bResult)
		{
			mTexture = nullptr;
			mDesc = {};
		}

		return bResult;
	}

	bool Texture::create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture)
	{
		mTexture = _texture;
		mTexture->GetDesc(&mDesc);

		bool Result = CreateView();
		
		if (false == Result)
		{
			mTexture = nullptr;
			mDesc = {};
		}

		return Result;
	}

	bool Texture::create(const D3D11_TEXTURE2D_DESC& _TexDesc)
	{
		bool Result = false;
		mDesc = _TexDesc;

		Result = SUCCEEDED(RenderManager::get_inst().Device()->CreateTexture2D(&mDesc, nullptr, mTexture.GetAddressOf()));
		if (false == Result)
		{
			ERROR_MESSAGE("텍스처 생성에 실패 했습니다.");
			mDesc = {};
			return false;
		}

		Result = CreateView();
		if(false == Result)
		{
			mDesc = {};
			Result = false;
		}
		
		return Result;
	}

	eResult Texture::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
	{
		return SaveFile(_base_directory / _resource_name);
	}

	eResult Texture::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		std::fs::path fullPath = _base_directory / _resource_name;

		if (false == std::fs::exists(fullPath))
		{
			std::wstring errMsg = fullPath.wstring() + L"\n";
			errMsg += L"파일이 없습니다.";
			ERROR_MESSAGE_W(errMsg.c_str());
			return eResult::Fail_Open;
		}

		eResult result = LoadFile(fullPath);

		if (eResult::Success == result)
		{
			InitializeResource();
		}
		return result;
	}

	eResult Texture::LoadFile(const std::filesystem::path& _fullPath)
	{
		std::wstring Extension = _fullPath.extension().wstring();
		StringConverter::MakeUpperCase(Extension);

		if (Extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(_fullPath.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return eResult::Fail_Open;
		}
		else if (Extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(_fullPath.c_str(), DirectX::TGA_FLAGS::TGA_FLAGS_NONE, nullptr, mImage)))
				return eResult::Fail_Open;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(_fullPath.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return eResult::Fail_Open;
		}

		return eResult::Success;
	}

	eResult Texture::SaveFile(const std::filesystem::path& _fullPath) const
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

		std::wstring Extension = _fullPath.extension().wstring();
		
		DirectX::ScratchImage img{};
		if (FAILED(DirectX::CaptureTexture(RenderManager::get_inst().Device(), RenderManager::get_inst().Context(), mTexture.Get(), img)))
		{
			ERROR_MESSAGE("Texture를 Scratch Image로 가져오는 데 실패했습니다.");
			return eResult::Fail_Create;
		}

		StringConverter::MakeUpperCase(Extension);
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

	void Texture::InitializeResource()
	{
		CreateShaderResourceView
		(
			RenderManager::get_inst().Device(),
			mImage.GetImages(),
			mImage.GetImageCount(),
			mImage.GetMetadata(),
			mSRV.GetAddressOf()
		);

		mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());
		mTexture->GetDesc(&mDesc);
	}

	void Texture::bind_buffer_as_SRV(uint _SRVSlot, eShaderStageFlag_ _stageFlag)
	{
		unbind_buffer_from_GPU_register();
	
		mCurBoundRegister = (int)_SRVSlot;
		mCurBoundStage = _stageFlag;
		mCurBoundView = eBufferViewType::SRV;

		auto pContext = RenderManager::get_inst().Context();
		if (eShaderStageFlag::Vertex & _stageFlag)
		{
			pContext->VSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
		if (eShaderStageFlag::Hull & _stageFlag)
		{
			pContext->HSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
		if (eShaderStageFlag::Domain & _stageFlag)
		{
			pContext->DSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
		if (eShaderStageFlag::Geometry & _stageFlag)
		{
			pContext->GSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
		if (eShaderStageFlag::Pixel & _stageFlag)
		{
			pContext->PSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
		if (eShaderStageFlag::Compute & _stageFlag)
		{
			pContext->CSSetShaderResources(_SRVSlot, 1u, mSRV.GetAddressOf());
		}
	}

	void Texture::bind_buffer_as_UAV(uint _startSlot)
	{
		unbind_buffer_from_GPU_register();

		mCurBoundView = eBufferViewType::UAV;
		mCurBoundRegister = (int)_startSlot;

		uint i = -1;
		RenderManager::get_inst().Context()->CSSetUnorderedAccessViews(_startSlot, 1, mUAV.GetAddressOf(), &i);
	}


	void Texture::unbind_buffer_from_GPU_register()
	{
		switch (mCurBoundView)
		{
		case core::eBufferViewType::NONE:
			break;

		case core::eBufferViewType::SRV:
		{
			ASSERT(0 <= mCurBoundRegister, "Bound된 레지스터 번호가 음수입니다.");
			ID3D11ShaderResourceView* srv = nullptr;

			auto pContext = RenderManager::get_inst().Context();

			if (eShaderStageFlag::Vertex & mCurBoundStage)
			{
				pContext->VSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}
			if (eShaderStageFlag::Hull & mCurBoundStage)
			{
				pContext->HSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}
			if (eShaderStageFlag::Domain & mCurBoundStage)
			{
				pContext->DSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}
			if (eShaderStageFlag::Geometry & mCurBoundStage)
			{
				pContext->GSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}
			if (eShaderStageFlag::Pixel & mCurBoundStage)
			{
				pContext->PSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}
			if (eShaderStageFlag::Compute & mCurBoundStage)
			{
				pContext->CSSetShaderResources(mCurBoundRegister, 1u, &srv);
			}

			mCurBoundRegister = -1;
			mCurBoundStage = eShaderStageFlag::NONE;
			
			break;
		}
		case core::eBufferViewType::UAV:
		{
			ID3D11UnorderedAccessView* pUAV = nullptr;
			uint u = -1;

			RenderManager::get_inst().Context()->CSSetUnorderedAccessViews(mCurBoundRegister, 1, &pUAV, &u);

			//현재 연결된 레지스터 번호와 파이프라인을 초기화
			mCurBoundRegister = -1;
			mCurBoundStage = eShaderStageFlag::NONE;

			break;
		}
			
		case core::eBufferViewType::RTV:
			[[fallthrough]];
		case core::eBufferViewType::DSV:
		{
			ID3D11RenderTargetView* pRTV = nullptr;
			ID3D11DepthStencilView* pDSV = nullptr;

			RenderManager::get_inst().Context()->OMSetRenderTargets(1u, &pRTV, pDSV);
			break;
		}

		default:
			break;
		}


		mCurBoundView = eBufferViewType::NONE;
	}

	bool Texture::CreateView()
	{
		auto pDevice = RenderManager::get_inst().Device();
		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (FAILED(pDevice->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf())))
			{
				ERROR_MESSAGE("Depth Stencil View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(pDevice->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf())))
			{
				ERROR_MESSAGE("Render Target View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = mDesc.Format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			if (FAILED(pDevice->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf())))
			{
				ERROR_MESSAGE("Shader Resource View 생성에 실패 했습니다.");
				return false;
			}
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = mDesc.Format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			if (FAILED(pDevice->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf())))
			{
				ERROR_MESSAGE("Unordered Access View 생성에 실패 했습니다.");
				return false;
			}
		}

		return true;
	}


}
