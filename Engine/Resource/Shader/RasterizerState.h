#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class RasterizerState final :
        public Resource
    {
		CLASS_INFO(RasterizerState, Resource);
        BASE_RESOURCE(RasterizerState, eResourceType::UNKNOWN);

    public:
        RasterizerState();
        virtual ~RasterizerState();
        virtual eResult save(const std::fs::path& _full_path) const override;
		virtual eResult load(const std::fs::path& _full_path) override;

		void set_rasterizer_state(ComPtr<ID3D11RasterizerState> _rasterizer_state) 
        { 
            m_rasterizer_state = std::move(_rasterizer_state); 
        }
        bool create_rasterizer_state(const D3D11_RASTERIZER_DESC& _rs_desc);

        void bind(const ComPtr<ID3D11DeviceContext>& _context);
        
    private:
        ComPtr<ID3D11RasterizerState> m_rasterizer_state = {};
    };
}

