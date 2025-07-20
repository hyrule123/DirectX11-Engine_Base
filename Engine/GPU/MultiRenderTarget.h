#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/GPU/CommonGPU.h>

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
		void bind();
		s_ptr<Texture> get_render_target(uint _idx) {
			if (_idx >= MRT_MAX) { return nullptr; }
			return m_render_targets[_idx];
		}

		//렌더타겟을 특정 색상으로 변경
		void fill(const float4& _clearColor);

	private:
		s_ptr<Texture>	m_render_targets[MRT_MAX];
		ID3D11RenderTargetView*		m_render_target_views[MRT_MAX];
		s_ptr<Texture>	m_depth_stencil_resource;
		uint m_render_target_count;
    };
}


