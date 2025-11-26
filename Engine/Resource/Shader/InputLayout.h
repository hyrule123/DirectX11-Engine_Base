#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/GPU/CommonGPU.h"

namespace core
{
    class VertexShader;

    class InputLayout :
        public Resource
    {
    public:
        InputLayout();
        virtual ~InputLayout();
        virtual eResult save(const std::fs::path& _full_path) const override;
        virtual eResult load(const std::fs::path& _full_path) override;

        bool create_input_layout(
            const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs, 
            const std::shared_ptr<class VertexShader>& _vs
        );

        void bind(const ComPtr<ID3D11DeviceContext>& _context);

    private:
        ComPtr<ID3D11InputLayout> m_input_layout = {};

        std::vector<std::string> m_semantic_names = {};
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_input_layout_descs = {};

        w_ptr<VertexShader> m_vs = {};
    };
}

