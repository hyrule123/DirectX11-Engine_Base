#include "Engine/PCH_Engine.h"
#include "DepthStencilState.h"

namespace core
{
	DepthStencilState::DepthStencilState()
	{
	}
	DepthStencilState::~DepthStencilState()
	{
	}
	eResult DepthStencilState::save(const std::fs::path& _full_path) const
	{
		ASSERT_MESSAGE_RELEASE(false, "Not Implemented");
		return eResult::Fail;
	}
	eResult DepthStencilState::load(const std::fs::path& _full_path)
	{
		ASSERT_MESSAGE_RELEASE(false, "Not Implemented");
		return eResult::Fail;
	}
	void DepthStencilState::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), m_stencil_ref);
	}
}
