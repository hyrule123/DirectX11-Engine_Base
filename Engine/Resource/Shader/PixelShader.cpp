#include "Engine/PCH_Engine.h"
#include "PixelShader.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	PixelShader::PixelShader()
	{
	}
	PixelShader::~PixelShader()
	{
	}

	void PixelShader::bind_shader(const ComPtr<ID3D11DeviceContext>& _context)
	{

		_context->PSSetShader(m_shader.Get(), nullptr, 0);
	}

	bool PixelShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreatePixelShader(
			_byte_code.data(),
			_byte_code.size(),
			nullptr,
			m_shader.GetAddressOf()
		);

		if (FAILED(result))
		{
			ERROR_MESSAGE("픽셀 쉐이더 생성에 실패했습니다.");
			return false;
		}

		return true;
	}
}
