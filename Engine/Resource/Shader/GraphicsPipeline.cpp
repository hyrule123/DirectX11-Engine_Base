#include "GraphicsPipeline.h"

#include "Engine/Global.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Resource/Shader/VertexShader.h"
#include "Engine/Resource/Shader/HullShader.h"
#include "Engine/Resource/Shader/DomainShader.h"
#include "Engine/Resource/Shader/GeometryShader.h"
#include "Engine/Resource/Shader/PixelShader.h"

namespace core
{
	GraphicsPipeline::GraphicsPipeline()
		: Resource()
	{
		m_sample_mask = UINT_MAX;
		m_stencil_ref = UINT_MAX;

		m_vertex_shader = new_entity<VertexShader>();
		m_hull_shader = new_entity<HullShader>();
		m_domain_shader = new_entity<DomainShader>();
		m_geometry_shader = new_entity<GeometryShader>();
		m_pixel_shader = new_entity<PixelShader>();
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
	}

	eResult GraphicsPipeline::save(const std::fs::path& _full_path) const
	{
		return save_file_json(_full_path);
	}

	eResult GraphicsPipeline::load(const std::fs::path& _full_path)
	{
		return load_file_json(_full_path);
	}


	void GraphicsPipeline::bind_shader()
	{
		auto context = RenderManager::get_inst().get_context();
		
		m_vertex_shader->bind_shader(context);
		m_hull_shader->bind_shader(context);
		m_domain_shader->bind_shader(context);
		m_geometry_shader->bind_shader(context);
		m_pixel_shader->bind_shader(context);

		pContext->RSSetState(m_rasterizer_state.Get());
		pContext->OMSetBlendState(m_blend_state.Get(), m_blend_factor, m_sample_mask);
		pContext->OMSetDepthStencilState(m_depth_stencil_state.Get(), 10u);
	}

	void GraphicsPipeline::unbind_all_shader()
	{
		auto pContext = RenderManager::get_inst().get_context();

		pContext->IASetInputLayout(nullptr);
		pContext->VSSetShader(nullptr, nullptr, 0);
		pContext->HSSetShader(nullptr, nullptr, 0);
		pContext->DSSetShader(nullptr, nullptr, 0);
		pContext->GSSetShader(nullptr, nullptr, 0);
		pContext->PSSetShader(nullptr, nullptr, 0);

		pContext->RSSetState(nullptr);
		pContext->OMSetDepthStencilState(nullptr, 10u);
		pContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
	}

	eResult GraphicsPipeline::serialize_json(JsonSerializer& _ser) const
	{
		eResult result = Super::serialize_json(_ser);
		if (eResult_fail(result))
		{
			return result;
		}

		//구조체 안에 const char* 타입이 있어서 수동으로 해줘야함
		Json::Value& jsonInputLayouts = _ser[JSON_KEY(m_input_layout_descs)];
		jsonInputLayouts = Json::Value(Json::arrayValue);

		//Input Layout Desc
		for (size_t i = 0; i < m_input_layout_descs.size(); ++i)
		{
			Json::Value& InputElement = jsonInputLayouts[i];

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticName)]		
				<< m_input_layout_descs[i].SemanticName;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticIndex)]		
				<< m_input_layout_descs[i].SemanticIndex;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::Format)]				
				<< m_input_layout_descs[i].Format;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlot)]			
				<< m_input_layout_descs[i].InputSlot;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::AlignedByteOffset)]	
				<< m_input_layout_descs[i].AlignedByteOffset;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlotClass)]		
				<< m_input_layout_descs[i].InputSlotClass;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InstanceDataStepRate)] 
				<< m_input_layout_descs[i].InstanceDataStepRate;
		}

		//쉐이더 파일명
		{
			Json::Value& ShaderFileName = _ser[JSON_KEY(m_shader_codes)];
			for (int i = 0; i < (int)eGSStage::END; ++i)
			{
				ShaderFileName.append(m_shader_codes[i].name);
			}
		}

		//래스터라이저 상태
		_ser[JSON_KEY(m_rasterizer_type)] << m_rasterizer_type;
		_ser[JSON_KEY(m_blend_type)] << m_blend_type;
		_ser[JSON_KEY(m_depth_stencil_type)] << m_depth_stencil_type;

		return eResult::Success;
	}


	eResult GraphicsPipeline::deserialize_json(const JsonSerializer& _ser)
	{
		eResult result = Super::deserialize_json(_ser);
		if (eResult_fail(result))
		{
			return result;
		}

		//Input Layout Desc
		m_input_layout_descs.clear();
		if (_ser.isMember(JSON_KEY(m_input_layout_descs)))
		{
			const Json::Value& jsonInputLayouts = _ser[JSON_KEY(m_input_layout_descs)];

			if (jsonInputLayouts.isArray())
			{
				for (
					Json::ValueConstIterator iter = jsonInputLayouts.begin();
					iter != jsonInputLayouts.end();
					++iter
					)
				{
					const Json::Value& inputElement = *iter;

					//멤버 전부 있는지 확인
					bool checkMember =
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticIndex)) &&
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::Format)) &&
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlot)) &&
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::AlignedByteOffset)) &&
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlotClass)) &&
						inputElement.isMember(JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InstanceDataStepRate));

					if (false == checkMember)
					{
						ERROR_MESSAGE("JSON 파일에 로드에 필요한 데이터가 누락되어 있습니다.");
						return eResult::Fail_InValid;
					}

					D3D11_INPUT_ELEMENT_DESC& desc = m_input_layout_descs.emplace_back(D3D11_INPUT_ELEMENT_DESC{});

					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticName)] >> desc.SemanticName;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticIndex)] >> desc.SemanticIndex;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::Format)] >> desc.Format;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlot)] >> desc.InputSlot;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::AlignedByteOffset)] >> desc.AlignedByteOffset;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlotClass)] >> desc.InputSlotClass;
					inputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InstanceDataStepRate)] >> desc.InstanceDataStepRate;
				}
			}
		}

		//쉐이더
		{
			const Json::Value& shaderFileNames = _ser[JSON_KEY(m_shader_codes)];

			if ((size_t)shaderFileNames.size() < m_shader_codes.size())
			{
				ERROR_MESSAGE("쉐이더 파일명이 누락되어 있습니다.");
				return eResult::Fail_InValid;
			}
			
			for (size_t i = 0; i < m_shader_codes.size(); ++i)
			{
				const Json::Value& shaderFileName = shaderFileNames[i];
				if (false == shaderFileName.isString())
				{
					ERROR_MESSAGE("문자열이 들어와야 하는데 문자열 형식이 아닙니다.");
					return eResult::Fail_InValid;
				}

				m_shader_codes[i].name = shaderFileName.asString();

				//에딧 모드가 아닐 경우에만 로드
				if (false == m_b_edit_mode && false == m_shader_codes[i].name.empty())
				{
					eResult result = compile_from_CSO((eGSStage)i, m_shader_codes[i].name);
					if (eResult_fail(result))
					{
						ERROR_MESSAGE("쉐이더 코드 생성 실패");
						return result;
					}
				}
			}
		}

		//래스터라이저 상태
		_ser[JSON_KEY(m_rasterizer_type)] >> m_rasterizer_type;
		_ser[JSON_KEY(m_blend_type)] >> m_blend_type;
		_ser[JSON_KEY(m_depth_stencil_type)] >> m_depth_stencil_type;
		set_rasterizer_state(m_rasterizer_type);
		set_blend_state(m_blend_type);
		set_depth_stencil_state(m_depth_stencil_type);

		//에디트 모드가 아닐 경우에만 Input Layout 생성.
		if (false == m_b_edit_mode)
		{
			eResult result = create_input_layout();
			if (eResult_fail(result))
			{
				ERROR_MESSAGE("INPUT LAYOUT 생성 실패");
				return result;
			}
		}

		return eResult::Success;
	}

	eResult GraphicsPipeline::create_shader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize)
	{
		ASSERT_MESSAGE(_pByteCode, "ByteCode 주소가 nullptr입니다.");
		ASSERT_MESSAGE(_ByteCodeSize, "ByteCode의 사이즈가 0입니다.");

		auto pDevice = RenderManager::get_inst().get_device();
		switch (_stage)
		{
		case eGSStage::Vertex:
		{
			if (FAILED(pDevice->CreateVertexShader(_pByteCode, _ByteCodeSize, nullptr, m_vertex_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Vertex GraphicsPipeline 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		case eGSStage::Hull:
		{
			if (FAILED(pDevice->CreateHullShader(_pByteCode, _ByteCodeSize, nullptr, m_hull_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Hull GraphicsPipeline 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Domain:
		{
			if (FAILED(pDevice->CreateDomainShader(_pByteCode, _ByteCodeSize, nullptr, m_domain_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Domain GraphicsPipeline 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Geometry:
		{
			if (FAILED(pDevice->CreateGeometryShader(_pByteCode, _ByteCodeSize, nullptr, m_geometry_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Geometry GraphicsPipeline 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Pixel:
		{
			if (FAILED(pDevice->CreatePixelShader(_pByteCode, _ByteCodeSize, nullptr, m_pixel_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Pixel GraphicsPipeline 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		}

		return eResult::Success;
	}

}
