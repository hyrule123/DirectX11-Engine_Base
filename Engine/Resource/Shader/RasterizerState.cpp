#include "Engine/PCH_Engine.h"
#include "RasterizerState.h"

namespace core
{
	RasterizerState::RasterizerState()
	{
	}
	RasterizerState::~RasterizerState()
	{
	}
	eResult RasterizerState::save(const std::fs::path& _full_path) const
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}
	eResult RasterizerState::load(const std::fs::path& _full_path)
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}

	void RasterizerState::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->RSSetState(m_rasterizer_state.Get());
	}
}
