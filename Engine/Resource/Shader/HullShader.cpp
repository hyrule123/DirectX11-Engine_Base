
#include "Engine/PCH_Engine.h"
#include "HullShader.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	HullShader::HullShader()
	{
	}

	HullShader::~HullShader()
	{
	}

	void HullShader::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->HSSetShader(m_shader.Get(), nullptr, 0);
	}

	bool HullShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		const auto& device = RenderManager::get_inst().get_device();
		
		HRESULT result = device->CreateHullShader(
			_byte_code.data(),
			_byte_code.size(),
			nullptr,
			m_shader.GetAddressOf()
		);

		if (FAILED(result))
		{
			ERROR_MESSAGE("Hull Shader 생성 실패.");
			return false;
		}

		return true;
	}
}

