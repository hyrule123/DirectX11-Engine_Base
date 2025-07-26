
#include "GraphicsShader.h"

#include <Engine/Global.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/PathManager.h>

#include <Engine/Util/Serialize/JsonSerializer.h>

namespace core
{
	GraphicsShader::GraphicsShader()
		: Shader()
		, m_input_layout_descs{}
		, m_input_layout{}
		, m_shader_codes{}
		, m_vertex_shader{}
		, m_domain_shader{}
		, m_hull_shader{}
		, m_geometry_shader{}
		, m_pixel_shader{}
		, m_rasterizer_state{}
		, m_blend_state{}
		, m_depth_stencil_state{}
		, m_rasterizer_type{}
		, m_blend_type{}
		, m_depth_stencil_type{}
		, m_blend_factor{}
		, m_sample_mask{ UINT_MAX }
		, m_stencil_ref{ UINT_MAX }
		, m_error_blob()
		, m_b_edit_mode(false)
	{
		m_rasterizer_state = RenderManager::get_inst().get_rasterizer_state(m_rasterizer_type);
		m_blend_state = RenderManager::get_inst().get_blend_state(m_blend_type);
		m_depth_stencil_state = RenderManager::get_inst().get_depth_stencil_state(m_depth_stencil_type);
	}

	GraphicsShader::~GraphicsShader()
	{
	}

	eResult GraphicsShader::save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const
	{
		return save_file_json(_base_directory / _resource_name);
	}

	eResult GraphicsShader::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		return load_file_json(_base_directory / _resource_name);
	}

	eResult GraphicsShader::compile_from_source_code(eGSStage _stage, const std::fs::path& _FullPath, const std::string_view _funcName)
	{
		m_shader_codes[(int)_stage] = {};

		HRESULT hr = D3DCompileFromFile(
			_FullPath.wstring().c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			std::string(_funcName).c_str(),
			SHADER_VERSION::Graphics[(int)_stage],
			0u,
			0u,
			m_shader_codes[(int)_stage].blob.ReleaseAndGetAddressOf(),
			m_error_blob.ReleaseAndGetAddressOf()
		);

		if (FAILED(hr))
		{
			std::string ErrMsg = "Failed to compile Graphics GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(m_error_blob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());

			m_error_blob = nullptr;

			return eResult::Fail_Open;
		}

		m_shader_codes[(int)_stage].pData = 
			m_shader_codes[(int)_stage].blob.Get()->GetBufferPointer();
		return eResult::Success;
	}

	eResult GraphicsShader::compile_from_byte_code(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		m_shader_codes[(int)_stage] = {};

		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		set_engine_default_res(true);

		tShaderCode& code = m_shader_codes[(int)_stage];
		code.pData = _pByteCode;
		code.dataSize = _ByteCodeSize;

		return create_shader(_stage, code.pData, code.dataSize);
	}

	eResult GraphicsShader::compile_from_CSO(eGSStage _stage, const std::fs::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathManager::get_inst().get_relative_shader_directory();
		shaderBinPath /= _FileName;

		if (false == std::fs::exists(shaderBinPath))
		{
			return eResult::Fail_Open;
		}

		//위에서 만든 파일명을 토대로 디스크에서 파일을 열어준다.
		std::ios_base::openmode openFlag = std::ios_base::ate | std::ios_base::binary | std::ios_base::in;
		std::ifstream sFile(shaderBinPath, openFlag);

		if (false == sFile.is_open())
		{
			std::wstring msg = L"Failed to open File.\n";
			msg += shaderBinPath.wstring();
			ERROR_MESSAGE_W(msg.c_str());
		}

		//파일이 열리면 지역변수 Blob을 만들어서 데이터를 옮긴다.
		tShaderCode& code = m_shader_codes[(int)_stage];
		code = {};
		code.name = _FileName.string();
		code.dataSize = sFile.tellg();

		//Blob 내부에 공간을 할당.
		if (FAILED(D3DCreateBlob(code.dataSize, code.blob.GetAddressOf())))
		{
			code = {};
			ERROR_MESSAGE("쉐이더를 저장할 공간 할당에 실패했습니다.");
			return eResult::Fail_Create;
		}

		//커서를 처음으로 돌린 후 파일을 읽어준다.
		sFile.seekg(0, std::ios_base::beg);
		sFile.read((char*)code.blob->GetBufferPointer(), code.blob->GetBufferSize());
		sFile.close();

		//읽어온 바이트 코드로부터 쉐이더를 로딩해준다.
		//실패시 동적할당 해제하고 오류 발생
		eResult Result = create_shader(_stage, code.blob->GetBufferPointer(), code.blob->GetBufferSize());
		if (eResult_fail(Result))
		{
			code = {};
			ERROR_MESSAGE("쉐이더 생성 실패.");
			return Result;
		}
 
		code.pData = code.blob->GetBufferPointer();

		return Result;
	}

	void GraphicsShader::add_input_layout_desc(const D3D11_INPUT_ELEMENT_DESC& _desc)
	{
		m_input_layout_descs.push_back(_desc);

		//이름을 별도 저장된 공간에 저장된 뒤 해당 주소의 포인터로 교체
		const auto& pair = g_string_archive.insert(m_input_layout_descs.back().SemanticName);
		m_input_layout_descs.back().SemanticName = pair.first->c_str();
	}

	void GraphicsShader::set_input_layout_desc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs)
	{
		m_input_layout_descs = _descs;

		for (size_t i = 0; i < m_input_layout_descs.size(); ++i)
		{
			if (m_input_layout_descs[i].SemanticName)
			{
				const auto& pair = g_string_archive.insert(m_input_layout_descs[i].SemanticName);
				m_input_layout_descs[i].SemanticName = pair.first->c_str();
			}
		}
	}

	eResult GraphicsShader::create_input_layout()
	{
		const tShaderCode& VS = m_shader_codes[(int)eGSStage::Vertex];

		if (nullptr == VS.pData)
		{
			ERROR_MESSAGE("정점 쉐이더가 준비되지 않아서 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}
		else if (m_input_layout_descs.empty())
		{
			ERROR_MESSAGE("입력 레이아웃이 설정되어있지 않아 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}

		if (FAILED(RenderManager::get_inst().get_device()->CreateInputLayout(
			m_input_layout_descs.data(),
			(uint)m_input_layout_descs.size(),
			VS.pData,
			VS.dataSize,
			m_input_layout.ReleaseAndGetAddressOf()
		)))
		{
			ERROR_MESSAGE("Input Layout 생성에 실패했습니다.");
			return eResult::Fail_Create;
		}


		return eResult::Success;
	}

	void GraphicsShader::set_rasterizer_state(eRasterizerState _state)
	{
		m_rasterizer_type = _state;
		m_rasterizer_state = RenderManager::get_inst().get_rasterizer_state(_state);
	}

	void GraphicsShader::set_depth_stencil_state(eDepthStencilState _state)
	{
		m_depth_stencil_type = _state;
		m_depth_stencil_state = RenderManager::get_inst().get_depth_stencil_state(_state);
	}

	void GraphicsShader::set_blend_state(eBlendState _state)
	{
		m_blend_type = _state;
		m_blend_state = RenderManager::get_inst().get_blend_state(_state);
	}

	void GraphicsShader::bind_shader()
	{
		auto pContext = RenderManager::get_inst().get_context();
		
		pContext->IASetInputLayout(m_input_layout.Get());
		pContext->VSSetShader(m_vertex_shader.Get(), nullptr, 0);
		pContext->HSSetShader(m_hull_shader.Get(), nullptr, 0);
		pContext->DSSetShader(m_domain_shader.Get(), nullptr, 0);
		pContext->GSSetShader(m_geometry_shader.Get(), nullptr, 0);
		pContext->PSSetShader(m_pixel_shader.Get(), nullptr, 0);

		pContext->RSSetState(m_rasterizer_state.Get());
		pContext->OMSetBlendState(m_blend_state.Get(), m_blend_factor, m_sample_mask);
		pContext->OMSetDepthStencilState(m_depth_stencil_state.Get(), 10u);
	}

	void GraphicsShader::unbind_all_shader()
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

	eResult GraphicsShader::serialize_json(JsonSerializer& _ser) const
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


	eResult GraphicsShader::deserialize_json(const JsonSerializer& _ser)
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

	eResult GraphicsShader::create_shader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize)
	{
		ASSERT(_pByteCode, "ByteCode 주소가 nullptr입니다.");
		ASSERT(_ByteCodeSize, "ByteCode의 사이즈가 0입니다.");

		auto pDevice = RenderManager::get_inst().get_device();
		switch (_stage)
		{
		case eGSStage::Vertex:
		{
			if (FAILED(pDevice->CreateVertexShader(_pByteCode, _ByteCodeSize, nullptr, m_vertex_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Vertex GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		case eGSStage::Hull:
		{
			if (FAILED(pDevice->CreateHullShader(_pByteCode, _ByteCodeSize, nullptr, m_hull_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Hull GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Domain:
		{
			if (FAILED(pDevice->CreateDomainShader(_pByteCode, _ByteCodeSize, nullptr, m_domain_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Domain GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Geometry:
		{
			if (FAILED(pDevice->CreateGeometryShader(_pByteCode, _ByteCodeSize, nullptr, m_geometry_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Geometry GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Pixel:
		{
			if (FAILED(pDevice->CreatePixelShader(_pByteCode, _ByteCodeSize, nullptr, m_pixel_shader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Pixel GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		}

		return eResult::Success;
	}

}
