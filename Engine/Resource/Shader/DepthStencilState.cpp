#include "Engine/PCH_Engine.h"
#include "DepthStencilState.h"

#include "Engine/Manager/RenderManager.h"

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
	bool DepthStencilState::create_depth_stencil_state(const D3D11_DEPTH_STENCIL_DESC& _ds_desc)
	{
		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateDepthStencilState(
			&_ds_desc,
			m_depth_stencil_state.ReleaseAndGetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("Depth Stencil State 생성 실패.");
			return false;
		}
		return true;
	}
}
