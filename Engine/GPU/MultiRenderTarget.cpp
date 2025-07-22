#include "MultiRenderTarget.h"


#include <Engine/define_Macro.h>
#include <Engine/Resource/Texture.h>

namespace core
{
	MultiRenderTarget::MultiRenderTarget()
		: Entity()
		, m_render_targets{}
		, m_render_target_views{}
		, m_depth_stencil_resource{}
		, m_render_target_count{}
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
				m_render_target_count = i;
				break;
			}

			m_render_targets[i] = _texture[i];
			m_render_target_views[i] = _texture[i]->get_RTV().Get();
		}

		m_depth_stencil_resource = _dsTexture;

		return true;
	}


	void MultiRenderTarget::bind()
	{
		for (uint i = 0; i < m_render_target_count; ++i)
		{
			m_render_targets[i]->unbind_buffer_from_GPU_register();
		}

		ID3D11DepthStencilView* pDSView = nullptr;
		if (m_depth_stencil_resource)
		{
			pDSView = m_depth_stencil_resource->get_DSV().Get();
		}

		RenderManager::get_inst().get_context()->OMSetRenderTargets(m_render_target_count, m_render_target_views, pDSView);
	}

	void MultiRenderTarget::fill(const float4& _clearColor)
	{
		for (uint i = 0u; i < m_render_target_count; ++i)
		{
			if (m_render_target_views[i])
			{
				RenderManager::get_inst().get_context()->ClearRenderTargetView(m_render_target_views[i], (FLOAT*)&_clearColor);
			}
		}

		if (m_depth_stencil_resource)
		{
			constexpr uint ClearFlag = D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL;
			RenderManager::get_inst().get_context()->ClearDepthStencilView(m_depth_stencil_resource->get_DSV().Get(), ClearFlag, 1.f, (UINT8)0);
		}
	}

}
