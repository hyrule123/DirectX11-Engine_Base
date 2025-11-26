#include "Engine/PCH_Engine.h"
#include "RasterizerState.h"

#include "Engine/Manager/RenderManager.h"

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

	bool RasterizerState::create_rasterizer_state(const D3D11_RASTERIZER_DESC& _rs_desc)
	{
		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateRasterizerState(
			&_rs_desc,
			m_rasterizer_state.ReleaseAndGetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("RasterizerState 생성 실패");
			return false;
		}

		return true;
	}

	void RasterizerState::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->RSSetState(m_rasterizer_state.Get());
	}
}
