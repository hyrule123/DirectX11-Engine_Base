#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class SamplerState :
        public Resource
    {
		CLASS_INFO(SamplerState, Resource);
		BASE_RESOURCE(SamplerState, eResourceType::UNKNOWN);

        public:
        SamplerState();
		virtual ~SamplerState();

		virtual eResult save(const std::fs::path& _full_path) const override;
        virtual eResult load(const std::fs::path& _full_path) override;

        void bind(const ComPtr<ID3D11DeviceContext>& _context);

        void set_sampler_state(ComPtr<ID3D11SamplerState> _sampler_state)
        {
            m_sampler_state = std::move(_sampler_state);
        }
		void set_slot(UINT _slot) { m_slot = _slot; }

		bool create_sampler_state(const D3D11_SAMPLER_DESC& _sampler_desc);

    private:
		ComPtr<ID3D11SamplerState> m_sampler_state = {};
        UINT m_slot = {};
    };
}


