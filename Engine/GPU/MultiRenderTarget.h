#pragma once
#include "Engine/Game/Entity.h"

#include "CommonGPU.h"

namespace core
{
	class Texture;
    class MultiRenderTarget :
        public Entity
    {
		CLASS_INFO(MultiRenderTarget, Entity);
	public:
		MultiRenderTarget();
		virtual ~MultiRenderTarget();

		bool create(s_ptr<Texture> texture[MRT_MAX], s_ptr<Texture> dsTexture);
		void Bind();
		s_ptr<Texture> get_rendertarget(uint _uRenderTargetIndex);

		//렌더타겟을 특정 색상으로 변경
		void Clear(const float4& _clearColor);

	private:
		s_ptr<Texture>	mRenderTargetTextures[MRT_MAX];
		ID3D11RenderTargetView*		mRenderTargetViews[MRT_MAX];
		s_ptr<Texture>	mDSTexture;
		uint mRenderTargetCount;
    };

	inline s_ptr<Texture> MultiRenderTarget::get_rendertarget(uint _uRenderTargetIndex)
	{
		s_ptr<Texture> RenderTarget = nullptr;
		if (_uRenderTargetIndex < MRT_MAX)
		{
			RenderTarget = mRenderTargetTextures[_uRenderTargetIndex];
		}
		return RenderTarget;
	}
}


