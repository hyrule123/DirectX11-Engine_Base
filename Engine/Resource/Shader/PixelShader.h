#pragma once
#include "Shader.h"

namespace core
{
	class PixelShader final :
		public Shader
	{
		CLASS_INFO(PixelShader, Shader);

	public:
		PixelShader();
		virtual ~PixelShader();

		virtual void bind(const ComPtr<ID3D11DeviceContext>& _context) override;

	protected:
		virtual bool load_shader(const std::vector<unsigned char>& _byte_code);

	private:
		ComPtr<ID3D11PixelShader> m_shader = {};
	};
}
