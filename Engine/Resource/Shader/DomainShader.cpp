#include "Engine/PCH_Engine.h"
#include "DomainShader.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	DomainShader::DomainShader()
	{
	}
	DomainShader::~DomainShader()
	{
	}
	void DomainShader::bind_shader(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context.DSSetShader(m_shader.Get(), nullptr, 0);
	}
	bool DomainShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		const auto& device = RenderManager::get_inst().get_device();

		HRESULT result = device->CreateDomainShader(
			_byte_code.data(),
			_byte_code.size(),
			nullptr,
			m_shader.GetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("Domain Shader 생성 실패.");
			return false;
		}

		return true;
	}
}
