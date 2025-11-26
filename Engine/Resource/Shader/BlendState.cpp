#include "Engine/PCH_Engine.h"
#include "BlendState.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

namespace core
{
	BlendState::BlendState()
	{
		m_sample_mask = std::numeric_limits<UINT>::max();
	}
	BlendState::~BlendState()
	{
	}

	eResult BlendState::save(const std::fs::path& _full_path) const
	{
		ASSERT_MESSAGE_RELEASE(false, "Not Implemented");
		return eResult::Fail;
	}

	eResult BlendState::load(const std::fs::path& _full_path)
	{
		ASSERT_MESSAGE_RELEASE(false, "Not Implemented");
		return eResult::Fail;
	}

	void BlendState::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->OMSetBlendState(m_blend_state.Get(), m_blend_factor, m_sample_mask);
	}
	bool BlendState::create_blend_state(const D3D11_BLEND_DESC& _bs_desc)
	{
		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateBlendState(
			&_bs_desc,
			m_blend_state.ReleaseAndGetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("BlendState 생성 실패");
			return false;
		}

		return true;
	}
}
