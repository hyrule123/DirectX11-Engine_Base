#pragma once
#include "Engine/GPU/CommonGPU.h"
#include "Engine/Common.h"
#include "Engine/Util/StaticSingleton.h"

namespace ehw
{
	class Texture;
	class GPUManager : public StaticSingleton<GPUManager>
	{
		friend class StaticSingleton<GPUManager>;
		friend class GameEngine;

	public:
		__forceinline ID3D11Device*			Device() { return mDevice.Get(); }
		__forceinline ID3D11DeviceContext*	Context() { return mContext.Get(); }

		void ClearRenderTarget();// 화면 지워주기
		inline void Present(bool _bVSync = false);

		inline std::shared_ptr<Texture> GetRenderTargetTex();
		inline std::shared_ptr<Texture> GetDepthStencilBufferTex();

		bool SetResolution(UINT _ResolutionX, UINT _ResolutionY);
		UINT GetResolutionX() { return mResolutionX; }
		UINT GetResolutionY() { return mResolutionY; }
		uint2 GetResolution() { return uint2{ mResolutionX, mResolutionY }; }

	private:
		GPUManager();
		~GPUManager();
		bool Init(const tGPUManagerDesc& _Desc);
		void Release();
		void Reset();
		bool CreateDevice(ID3D11Device** _ppDevice, ID3D11DeviceContext** _ppContext);
		
		//스왑체인 + 렌더타겟 생성하여 반환
		std::shared_ptr<Texture> CreateSwapChain(UINT _ResolutionX, UINT _ResolutionY, UINT _RefreshRate);
		std::shared_ptr<Texture> CreateDepthStencil(UINT _ResolutionX, UINT _ResolutionY);

		//Application의 창 크기를 따라감
		void CreateMainViewPort();

		//mContext->OMSetRenderTargets(1, mRenderTargetTexture->GetRTV().GetAddressOf(), mDepthStencilBufferTexture->GetDSV().Get());

	private:
		UINT mResolutionX;
		UINT mResolutionY;
		UINT mRefreshRate;

		ComPtr<ID3D11Device> mDevice;
		ComPtr<ID3D11DeviceContext> mContext;
	
		ComPtr<IDXGISwapChain> mSwapChain;
		
		std::shared_ptr<ehw::Texture> mRenderTargetTexture;
		std::shared_ptr<ehw::Texture> mDepthStencilBufferTexture;
		
		D3D11_VIEWPORT mViewPort;
	};

	inline void GPUManager::Present(bool _bVSync)
	{
		mSwapChain->Present(_bVSync ? 1 : 0, 0u);
	}

	inline std::shared_ptr<ehw::Texture> GPUManager::GetRenderTargetTex()
	{
		return mRenderTargetTexture;
	}

	inline std::shared_ptr<ehw::Texture> GPUManager::GetDepthStencilBufferTex()
	{
		return mDepthStencilBufferTexture;
	}
}


