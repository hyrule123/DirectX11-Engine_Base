#include "Engine/PCH_Engine.h"
#include "SamplerState.h"

namespace core
{
    SamplerState::SamplerState()
    {
    }
    SamplerState::~SamplerState()
    {
    }
    eResult SamplerState::save(const std::fs::path& _full_path) const
    {
        ASSERT_MESSAGE_RELEASE(false, "미구현");
        return eResult::Fail;
    }
    eResult SamplerState::load(const std::fs::path& _full_path)
    {
        ASSERT_MESSAGE_RELEASE(false, "미구현");
        return eResult::Fail;
    }
    void SamplerState::bind(const ComPtr<ID3D11DeviceContext>& _context)
    {
		_context->PSSetSamplers(m_slot, 1, m_sampler_state.GetAddressOf());
    }
}
