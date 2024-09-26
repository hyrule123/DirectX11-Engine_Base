#pragma once
#include "Engine/Game/Entity.h"

#include "CommonGPU.h"

namespace ehw
{
	class Texture;
    class MultiRenderTarget :
        public Entity
    {
		CLASS_NAME(MultiRenderTarget);
	public:
		MultiRenderTarget();
		virtual ~MultiRenderTarget();

		bool create(std::shared_ptr<Texture> texture[MRT_MAX], std::shared_ptr<Texture> dsTexture);
		void Bind();
		inline std::shared_ptr<Texture> GetRenderTarget(uint _uRenderTargetIndex);

		//렌더타겟을 특정 색상으로 변경
		void Clear(const float4& _clearColor);

	private:
		std::shared_ptr<Texture>	mRenderTargetTextures[MRT_MAX];
		ID3D11RenderTargetView*		mRenderTargetViews[MRT_MAX];
		std::shared_ptr<Texture>	mDSTexture;
		uint mRenderTargetCount;
    };

	inline std::shared_ptr<Texture> MultiRenderTarget::GetRenderTarget(uint _uRenderTargetIndex)
	{
		std::shared_ptr<Texture> RenderTarget = nullptr;
		if (_uRenderTargetIndex < MRT_MAX)
		{
			RenderTarget = mRenderTargetTextures[_uRenderTargetIndex];
		}
		return RenderTarget;
	}
}


