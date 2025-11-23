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
		_context->IASetInputLayout(m_input_layout.Get());
		_context->VSSetShader(m_shader.Get(), nullptr, 0);
	}

	bool VertexShader::load_shader(const std::vector<unsigned char>&_byte_code)
	{
		HRESULT result = D3DCreateBlob(_byte_code.size(), m_byte_code.GetAddressOf());
		if (FAILED(result))
		{
			ERROR_MESSAGE("쉐이더 바이트 코드를 저장할 공간 할당에 실패했습니다.");
			return false;
		}

		std::memcpy(m_byte_code->GetBufferPointer(), _byte_code.data(), _byte_code.size());

		const auto& device = RenderManager::get_inst().get_device();

		result = device->CreateVertexShader(
			m_byte_code->GetBufferPointer(),
			m_byte_code->GetBufferSize(),
			nullptr,
			m_shader.GetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("버텍스 쉐이더 생성에 실패했습니다.");
			return false;
		}

		//Input Layout Description이 등록된 상태일 경우 Input Layout을 생성한다.
		if (!(m_input_layout_descs.empty()))
		{
			return create_input_layout_internal();
		}

		return true;
	}

	bool VertexShader::create_input_layout(const std::vector<D3D11_INPUT_ELEMENT_DESC>&_descs)
	{
		m_semantic_names.clear();
		m_input_layout_descs.clear();
		m_input_layout.Reset();

		//desc는 문자열을 c style로 저장하기때문에 댕글링 포인터 발생 가능성이 있음.
		//문자열을 따로 저장하고, desc의 문자열 포인터를 그걸 가리키도록 변경해준다.
		m_input_layout_descs = _descs;
		for (auto& desc : m_input_layout_descs)
		{
			m_semantic_names.push_back(desc.SemanticName);
			desc.SemanticName = m_semantic_names.back().c_str();
		}

		//Vertex Shader가 로딩되었을 경우 Input Layout을 바로 생성한다.
		if (m_byte_code)
		{
			return create_input_layout_internal();
		}

		return true;
	}
	bool VertexShader::create_input_layout_internal()
	{
		if (m_byte_code && !(m_input_layout_descs.empty()))
		{
			const auto& device = RenderManager::get_inst().get_device();
			HRESULT result = device->CreateInputLayout(
				m_input_layout_descs.data(),
				static_cast<UINT>(m_input_layout_descs.size()),
				m_byte_code->GetBufferPointer(),
				m_byte_code->GetBufferSize(),
				m_input_layout.GetAddressOf()
			);
			if (SUCCEEDED(result))
			{
				return true;
			}
			else
			{
				ERROR_MESSAGE("Input Layout 생성에 실패했습니다.");
			}
		}

		return false;
	}
}
