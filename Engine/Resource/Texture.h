#pragma once
#include "Engine/Resource/Resource.h"
#include "Engine/Manager/RenderManager.h"

#include <DirectXTex/DirectXTex.h>

namespace ehw
{
	class Texture final : public Resource
	{
		CLASS_NAME(Texture);
	public:
		Texture();
		virtual ~Texture();

		static void ClearSRV(UINT _startSlot);
		static void ClearAll();

		//생성 관련 함수
		bool create(UINT _width, UINT _height, DXGI_FORMAT _format, uint _D3D11_BIND_FLAG, bool _bAllowCPURead = false);
		bool create(ComPtr<ID3D11Texture2D> _texture);
		bool create(const D3D11_TEXTURE2D_DESC& _TexDesc);

		//Save / Load
		virtual eResult save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;
		void InitializeResource();


		void BindDataSRV(uint _SRVSlot, eShaderStageFlag_ _stageFlag);
		void BindDataUAV(uint _UAVSlot = 0u);
		void unbind_buffer_from_GPU_register();


		void set_texture(Microsoft::WRL::ComPtr<ID3D11Texture2D> _texture) { mTexture = _texture; }

		const D3D11_TEXTURE2D_DESC& GetDesc() const { return mDesc; }
		uint GetHeight() const { return mDesc.Height; }
		uint GetWidth() const { return mDesc.Width; }
		uint2 GetSizeUint() const { return uint2{ mDesc.Width, mDesc.Height }; }
		float2 GetSizeFloat() const { return float2{ (float)mDesc.Width, (float)mDesc.Height }; }

		ComPtr<ID3D11Texture2D> get_texture() { return mTexture; }
		ComPtr<ID3D11DepthStencilView> GetDSV() { return mDSV; }
		ComPtr<ID3D11RenderTargetView> GetRTV() { return mRTV; }
		ComPtr<ID3D11UnorderedAccessView> GetUAV() { return mUAV; }
		ComPtr<ID3D11ShaderResourceView> GetSRV() { return mSRV; }

	private:
		eResult SaveFile(const std::filesystem::path& _fullPath) const;
		eResult LoadFile(const std::filesystem::path& _fullPath);
		bool CreateView();

	private:
		D3D11_TEXTURE2D_DESC					mDesc;
		ComPtr<ID3D11Texture2D>					mTexture;
		DirectX::ScratchImage							mImage;
		
		//각종 View
		//특정 버퍼와 연결하기 위한 통행증이라고 보면 됨
		ComPtr<ID3D11ShaderResourceView>		mSRV;
		ComPtr<ID3D11UnorderedAccessView>		mUAV;
		ComPtr<ID3D11DepthStencilView>			mDSV;
		ComPtr<ID3D11RenderTargetView>			mRTV; 

		
		//현재 연결되어있는 버퍼 정보
		eBufferViewType							mCurBoundView;
		int										mCurBoundRegister;
		eShaderStageFlag_						mCurBoundStage;
	};
}
