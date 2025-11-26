#include "Engine/PCH_Engine.h"
#include "InputLayout.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Resource/Shader/VertexShader.h"


namespace core
{
	InputLayout::InputLayout()
	{
	}
	InputLayout::~InputLayout()
	{
	}
	eResult InputLayout::save(const std::fs::path& _full_path) const
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}
	eResult InputLayout::load(const std::fs::path& _full_path)
	{
		ASSERT_MESSAGE_RELEASE(false, "미구현");
		return eResult::Fail;
	}
	bool InputLayout::create_input_layout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs, const std::shared_ptr<class VertexShader>& _vs)
	{
		if (_descs.empty())
		{
			ERROR_MESSAGE("Desc EMPTY");
			return false;
		}
		if (nullptr == _vs)
		{
			ERROR_MESSAGE("Vertex Shader NULL");
			return false;
		}

		auto device = RenderManager::get_inst().get_device();
		HRESULT result = device->CreateInputLayout(
			_descs.data(),
			(UINT)_descs.size(),
			_vs->get_byte_code().data(),
			_vs->get_byte_code().size(),
			m_input_layout.GetAddressOf()
		);
		if (FAILED(result))
		{
			ERROR_MESSAGE("Input Layout 생성 실패.");
			return false;
		}

		//문자열의 경우 포인터가 증발할 수도 있으므로, 별도로 저장한다.
		m_semantic_names.clear();
		m_input_layout_descs = _descs;
		for (auto& desc : m_input_layout_descs)
		{
			std::string semantic = desc.SemanticName;
			m_semantic_names.push_back(semantic);
			desc.SemanticName = m_semantic_names.back().c_str();
		}
		
		return true;
	}
	void InputLayout::bind(const ComPtr<ID3D11DeviceContext>& _context)
	{
		ASSERT_MESSAGE(m_input_layout.Get(), "Input Layout Missing!");
		_context->IASetInputLayout(m_input_layout.Get());
	}
}
