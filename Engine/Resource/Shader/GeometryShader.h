#pragma once
#include "Shader.h"

namespace core
{
    class GeometryShader final :
        public Shader
    {
    public:
        GeometryShader();
        virtual ~GeometryShader();
		virtual void bind_shader(const ComPtr<ID3D11DeviceContext>& _context) override;

    protected:
		virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

    private:
		ComPtr<ID3D11GeometryShader> m_shader = {};
    };
}
