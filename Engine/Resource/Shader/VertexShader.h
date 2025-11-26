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

        const std::vector<unsigned char>& get_byte_code() const { return m_byte_code; }

    protected:
        virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

    private:
        ComPtr<ID3D11VertexShader> m_shader = {};
        std::vector<unsigned char> m_byte_code = {};
    };
}
