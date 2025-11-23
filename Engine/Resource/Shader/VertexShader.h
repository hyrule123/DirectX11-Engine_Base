#pragma once
#include "Engine/Resource/Shader/Shader.h"

namespace core
{
    class VertexShader final :
        public Shader
    {
		CLASS_INFO(VertexShader, Shader);
    public:
        VertexShader();
        virtual ~VertexShader();

		virtual void bind(const ComPtr<ID3D11DeviceContext>& _context) override;

    protected:
        virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

        bool create_input_layout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs);
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescs() { return m_input_layout_descs; }

    private:
        bool create_input_layout_internal();

		std::vector<std::string> m_semantic_names = {};
        std::vector<D3D11_INPUT_ELEMENT_DESC> m_input_layout_descs = {};
        ComPtr<ID3D11InputLayout> m_input_layout = {};

        ComPtr<ID3D11VertexShader> m_shader = {};

        //Vertex Shader은 Input Layout을 만들 때 다시 한 번 써야 하므로 쉐이더 데이터를 보관해야 한다.
        ComPtr<ID3DBlob> m_byte_code = {};
    };
}
