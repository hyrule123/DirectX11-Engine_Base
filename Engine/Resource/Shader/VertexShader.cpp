#include "Engine/PCH_Engine.h"
#include "VertexShader.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/Global.h"

#include <d3dcompiler.h>

namespace core
{
	VertexShader::VertexShader()
	{
	}

	VertexShader::~VertexShader()
	{
	}

	void VertexShader::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context->VSSetShader(m_shader.Get(), nullptr, 0);
	}

	bool VertexShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		const auto& device = RenderManager::get_inst().get_device();

		HRESULT result = device->CreateVertexShader(
			_byte_code.data(),
			(SIZE_T)_byte_code.size(),
			nullptr,
			m_shader.GetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("버텍스 쉐이더 생성에 실패했습니다.");
			return false;
		}

		//Input Layout 생성을 위한 바이트코드 보관
		m_byte_code = _byte_code;

		return true;
	}

}
