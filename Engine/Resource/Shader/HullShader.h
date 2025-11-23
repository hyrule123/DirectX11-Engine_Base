#pragma once
#include "Shader.h"

namespace core
{
    class HullShader final :
        public Shader
    {
    public:
        HullShader();
		virtual ~HullShader();

		virtual void bind(const ComPtr<ID3D11DeviceContext>& _context) override;

    protected:
		virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

    private:
		ComPtr<ID3D11HullShader> m_shader = {};
    };
}

