#include "Engine/GPU/MultiRenderTarget.h"

#include "Engine/define_Macro.h"
#include "Engine/Resource/Texture.h"

namespace core
{
	MultiRenderTarget::MultiRenderTarget()
		: Entity(MultiRenderTarget::s_static_type_name)
		, mRenderTargetTextures{}
		, mRenderTargetViews{}
		, mDSTexture{}
		, mRenderTargetCount{}
	{
	}
	MultiRenderTarget::~MultiRenderTarget()
	{
	}
	bool MultiRenderTarget::create(s_ptr<Texture> _texture[MRT_MAX], s_ptr<Texture> _dsTexture)
	{
		ASSERT(nullptr != _texture[0], "렌더타겟이 하나도 없습니다.");

		//렌더타겟의 갯수를 카운트
		for (int i = 0; i < MRT_MAX; ++i)
		{
			if (_texture[i].get() == nullptr)
			{
				mRenderTargetCount = i;
				break;
			}

			mRenderTargetTextures[i] = _texture[i];
			mRenderTargetViews[i] = _texture[i]->GetRTV().Get();
		}

		mDSTexture = _dsTexture;

		return true;
	}


	void MultiRenderTarget::Bind()
	{
		for (uint i = 0; i < mRenderTargetCount; ++i)
		{
			mRenderTargetTextures[i]->unbind_buffer_from_GPU_register();
		}

		ID3D11DepthStencilView* pDSView = nullptr;
		if (mDSTexture)
		{
			pDSView = mDSTexture->GetDSV().Get();
		}

		RenderManager::get_inst().Context()->OMSetRenderTargets(mRenderTargetCount, mRenderTargetViews, pDSView);
	}

	void MultiRenderTarget::Clear(const float4& _clearColor)
	{
		for (uint i = 0u; i < mRenderTargetCount; ++i)
		{
			if (mRenderTargetViews[i])
			{
				RenderManager::get_inst().Context()->ClearRenderTargetView(mRenderTargetViews[i], (FLOAT*)&_clearColor);
			}
		}

		if (mDSTexture)
		{
			constexpr uint ClearFlag = D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
			RenderManager::get_inst().Context()->ClearDepthStencilView(mDSTexture->GetDSV().Get(), ClearFlag, 1.f, (UINT8)0);
		}
	}

}
