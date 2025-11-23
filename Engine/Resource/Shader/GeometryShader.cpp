
#include "Engine/PCH_Engine.h"
#include "GeometryShader.h"

#include "Engine/Manager/RenderManager.h"

namespace core
{
	GeometryShader::GeometryShader()
	{
	}

	GeometryShader::~GeometryShader()
	{
	}

	void GeometryShader::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		_context.GSSetShader(m_shader.Get(), nullptr, 0);
	}

	bool GeometryShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		const auto& device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateGeometryShader(
			_byte_code.data(),
			_byte_code.size(),
			nullptr,
			m_shader.GetAddressOf()
		);
		
		if (FAILED(result))
		{
			ERROR_MESSAGE("지오메트리 쉐이더 생성에 실패했습니다.");
			return false;
		}

		return true;
	}
}

