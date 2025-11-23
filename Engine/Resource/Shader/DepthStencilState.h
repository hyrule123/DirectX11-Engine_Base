#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class DepthStencilState :
        public Resource
    {
		CLASS_INFO(DepthStencilState, Resource);
		BASE_RESOURCE(DepthStencilState, eResourceType::UNKNOWN);

    public:
        DepthStencilState();
        virtual ~DepthStencilState();
		virtual eResult save(const std::fs::path& _full_path) const override;
        virtual eResult load(const std::fs::path& _full_path) override;
        void bind(const ComPtr<ID3D11DeviceContext>& _context);

        void set_depth_stencil_state(ComPtr<ID3D11DepthStencilState> _depth_stencil_state)
        {
            m_depth_stencil_state = std::move(_depth_stencil_state);
		}

        void set_stencil_ref(UINT _stencil_ref) { m_stencil_ref = _stencil_ref; }

    private:
		ComPtr<ID3D11DepthStencilState> m_depth_stencil_state = {};
        UINT m_stencil_ref = {};
    };
}
