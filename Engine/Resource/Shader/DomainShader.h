#pragma once
#include "Engine/Resource/Shader/Shader.h"

namespace core
{
    class DomainShader final :
        public Shader
    {
    public:
        DomainShader();
		virtual ~DomainShader();
		virtual void bind_shader(const ComPtr<ID3D11DeviceContext>& _context) override;

    protected:
		virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

    private:
		ComPtr<ID3D11DomainShader> m_shader = {};
    };
}
