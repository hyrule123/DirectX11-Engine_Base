#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class BlendState final:
        public Resource
    {
		CLASS_INFO(BlendState, Resource);
		BASE_RESOURCE(BlendState, eResourceType::UNKNOWN);

    public:
        BlendState();
        virtual ~BlendState();

		virtual eResult save(const std::fs::path& _full_path) const override;
		virtual eResult load(const std::fs::path& _full_path) override;

        void bind(const ComPtr<ID3D11DeviceContext>& _context);

		bool create_blend_state(const D3D11_BLEND_DESC& _bs_desc);

        void set_blend_state(ComPtr<ID3D11BlendState> _blend_state)
        {
            m_blend_state = std::move(_blend_state);
		}
        void set_blend_factor(const FLOAT _blend_factor[4])
        {
            std::memcpy(m_blend_factor, _blend_factor, sizeof(FLOAT) * 4);
		}
        void set_sample_mask(UINT _sample_mask)
        {
            m_sample_mask = _sample_mask;
        }

    private:
		ComPtr<ID3D11BlendState> m_blend_state = {};

        FLOAT m_blend_factor[4] = {};
        UINT m_sample_mask = {};
    };
}

