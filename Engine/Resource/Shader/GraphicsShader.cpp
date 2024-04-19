#include "Engine/Resource/Shader/GraphicsShader.h"

#include "Engine/GlobalVariable.h"

#include "Engine/Manager/GPUManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

namespace ehw
{
	GraphicsShader::GraphicsShader()
		: iShader(typeid(GraphicsShader))
		, m_arrShaderCode{}
		, m_rasterizerType(eRSType::SolidBack)
		, m_depthStencilType(eDSType::Less)
		, m_blendType(eBSType::Default)
		, m_errorBlob()
		, m_bEditMode(false)
	{
	}

	GraphicsShader::~GraphicsShader()
	{

	}



	eResult GraphicsShader::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
	{
		return SaveFile_Json(_baseDir / _strKeyPath);
	}

	eResult GraphicsShader::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
	{
		return LoadFile_Json(_baseDir / _strKeyPath);
	}

	eResult GraphicsShader::CreateByCompile(eGSStage _stage, const std::fs::path& _FullPath, const std::string_view _funcName)
	{
		m_arrShaderCode[(int)_stage] = {};

		HRESULT hr = D3DCompileFromFile(
			_FullPath.wstring().c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			std::string(_funcName).c_str(),
			SHADER_VERSION::Graphics[(int)_stage],
			0u,
			0u,
			m_arrShaderCode[(int)_stage].blob.ReleaseAndGetAddressOf(),
			m_errorBlob.ReleaseAndGetAddressOf()
		);

		if (FAILED(hr))
		{
			std::string ErrMsg = "Failed to compile Graphics GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(m_errorBlob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());


			m_errorBlob = nullptr;

			return eResult::Fail_Open;
		}

		return eResult::Success;
	}

	eResult GraphicsShader::CreateByHeader(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		m_arrShaderCode[(int)_stage] = {};

		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		SetEngineDefaultRes(true);

		tShaderCode& code = m_arrShaderCode[(int)_stage];

		HRESULT hr = D3DCreateBlob(_ByteCodeSize, code.blob.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ERROR_MESSAGE("GraphicsShader를 저장할 Blob 생성에 실패했습니다.");
			
			return eResult::Fail_Create;
		}

		unsigned char* pCode = reinterpret_cast<unsigned char*>(code.blob->GetBufferPointer());
		//할당된 메모리 사이즈는 무조건 같아야 함
		size_t DestSize = code.blob->GetBufferSize();
		ASSERT(_ByteCodeSize == DestSize, "Shader 데이터 사이즈와 할당된 blob의 데이터 사이즈가 다릅니다.");

		//데이터 복사
		memcpy_s(pCode, DestSize, _pByteCode, _ByteCodeSize);

		return CreateShader(_stage, pCode, DestSize);
	}

	eResult GraphicsShader::CreateByCSO(eGSStage _stage, const std::fs::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathManager::GetShaderCSOPath();
		shaderBinPath /= _FileName;

		if (false == std::fs::exists(shaderBinPath))
		{
			return eResult::Fail_Open;
		}

		//위에서 만든 파일명을 토대로 디스크에서 파일을 열어준다.
		std::ios_base::openmode openFlag = std::ios_base::ate | std::ios_base::binary; std::ios_base::in;
		std::ifstream sFile(shaderBinPath, openFlag);

		if (false == sFile.is_open())
		{
			std::wstring msg = L"Failed to open File.\n";
			msg += shaderBinPath.wstring();
			ERROR_MESSAGE_W(msg.c_str());
		}

		//파일이 열리면 지역변수 Blob을 만들어서 데이터를 옮긴다.
		tShaderCode sCode = {};
		sCode.strKey = _FileName.string();

		//Blob 내부에 공간을 할당.
		if (FAILED(D3DCreateBlob(sFile.tellg(), sCode.blob.GetAddressOf())))
		{
			ERROR_MESSAGE("쉐이더를 저장할 공간 할당에 실패했습니다.");
			return eResult::Fail_Create;
		}

		//커서를 처음으로 돌린 후 파일을 읽어준다.
		sFile.seekg(0, std::ios_base::beg);
		sFile.read((char*)sCode.blob->GetBufferPointer(), sCode.blob->GetBufferSize());
		sFile.close();

		//읽어온 바이트 코드로부터 쉐이더를 로딩해준다.
		//실패시 동적할당 해제하고 오류 발생
		eResult Result = CreateShader(_stage, sCode.blob->GetBufferPointer(), sCode.blob->GetBufferSize());
		if (eResultFail(Result))
		{
			ERROR_MESSAGE("쉐이더 생성 실패.");
			return Result;
		}
 
		m_arrShaderCode[(int)_stage] = sCode;

		return Result;
	}

	void GraphicsShader::AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc)
	{
		m_inputLayoutDescs.push_back(_desc);

		//이름을 별도 저장된 공간에 저장된 뒤 해당 주소의 포인터로 교체
		const auto& pair = g_stringArchive.insert(m_inputLayoutDescs.back().SemanticName);
		m_inputLayoutDescs.back().SemanticName = pair.first->c_str();
	}

	void GraphicsShader::SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs)
	{
		m_inputLayoutDescs = _descs;

		for (size_t i = 0; i < m_inputLayoutDescs.size(); ++i)
		{
			if (m_inputLayoutDescs[i].SemanticName)
			{
				const auto& pair = g_stringArchive.insert(m_inputLayoutDescs[i].SemanticName);
				m_inputLayoutDescs[i].SemanticName = pair.first->c_str();
			}
		}
	}

	eResult GraphicsShader::CreateInputLayout()
	{
		ID3DBlob* VSBlobData = m_arrShaderCode[(int)eGSStage::Vertex].blob.Get();

		if (nullptr == VSBlobData)
		{
			ERROR_MESSAGE("정점 쉐이더가 준비되지 않아서 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}
		else if (m_inputLayoutDescs.empty())
		{
			ERROR_MESSAGE("입력 레이아웃이 설정되어있지 않아 Input Layout을 생성할 수 없습니다.");
			return eResult::Fail_Create;
		}

		if (FAILED(GPUManager::Device()->CreateInputLayout(
			m_inputLayoutDescs.data(),
			(uint)m_inputLayoutDescs.size(),
			VSBlobData->GetBufferPointer(),
			VSBlobData->GetBufferSize(),
			m_inputLayout.ReleaseAndGetAddressOf()
		)))
		{
			ERROR_MESSAGE("Input Layout 생성에 실패했습니다.");
			return eResult::Fail_Create;
		}


		return eResult::Success;
	}

	void GraphicsShader::BindData()
	{
		auto pContext = GPUManager::Context();
		
		pContext->IASetInputLayout(m_inputLayout.Get());
		pContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		pContext->HSSetShader(m_hullShader.Get(), nullptr, 0);
		pContext->DSSetShader(m_domainShader.Get(), nullptr, 0);
		pContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
		pContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		
		ID3D11RasterizerState*		rs = RenderManager::GetRasterizerState(m_rasterizerType);
		ID3D11DepthStencilState*	ds = RenderManager::GetDepthStencilState(m_depthStencilType);
		ID3D11BlendState*			bs = RenderManager::GetBlendState(m_blendType);

		pContext->RSSetState(rs);
		pContext->OMSetDepthStencilState(ds, 10u);

		constexpr float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
		pContext->OMSetBlendState(bs, blendFactor, UINT_MAX);
	}

	void GraphicsShader::UnbindData()
	{
		auto pContext = GPUManager::Context();

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

	eResult GraphicsShader::Serialize_Json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer& ser = *_ser;

		//구조체 안에 const char* 타입이 있어서 수동으로 해줘야함
		Json::Value& jsonInputLayouts = ser[JSON_KEY(m_inputLayoutDescs)];
		jsonInputLayouts = Json::Value(Json::arrayValue);

		//Input Layout Desc
		for (size_t i = 0; i < m_inputLayoutDescs.size(); ++i)
		{
			Json::Value& InputElement = jsonInputLayouts[i];

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticName)]		
				<< m_inputLayoutDescs[i].SemanticName;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::SemanticIndex)]		
				<< m_inputLayoutDescs[i].SemanticIndex;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::Format)]				
				<< m_inputLayoutDescs[i].Format;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlot)]			
				<< m_inputLayoutDescs[i].InputSlot;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::AlignedByteOffset)]	
				<< m_inputLayoutDescs[i].AlignedByteOffset;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InputSlotClass)]		
				<< m_inputLayoutDescs[i].InputSlotClass;

			InputElement[JSON_KEY(D3D11_INPUT_ELEMENT_DESC::InstanceDataStepRate)] 
				<< m_inputLayoutDescs[i].InstanceDataStepRate;
		}

		//Json::SaveLoad::SaveValueVector(_pJVal, JSON_KEY_PAIR(m_inputLayoutDescs));




		//쉐이더 파일명
		{
			Json::Value& ShaderFileName = ser[JSON_KEY(m_arrShaderCode)];
			for (int i = 0; i < (int)eGSStage::END; ++i)
			{
				ShaderFileName.append(m_arrShaderCode[i].strKey);
			}
		}

		//래스터라이저 상태
		ser[JSON_KEY(m_depthStencilType)] << m_rasterizerType;
		ser[JSON_KEY(m_depthStencilType)] << m_depthStencilType;
		ser[JSON_KEY(m_blendType)] << m_blendType;

		return eResult::Success;
	}


	eResult GraphicsShader::DeSerialize_Json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		//Input Layout Desc
		m_inputLayoutDescs.clear();
		if (ser.isMember(JSON_KEY(m_inputLayoutDescs)))
		{
			const Json::Value& jsonInputLayouts = ser[JSON_KEY(m_inputLayoutDescs)];

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

					D3D11_INPUT_ELEMENT_DESC& desc = m_inputLayoutDescs.emplace_back(D3D11_INPUT_ELEMENT_DESC{});

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
			const Json::Value& shaderFileNames = ser[JSON_KEY(m_arrShaderCode)];

			if ((size_t)shaderFileNames.size() < m_arrShaderCode.size())
			{
				ERROR_MESSAGE("쉐이더 파일명이 누락되어 있습니다.");
				return eResult::Fail_InValid;
			}
			
			for (size_t i = 0; i < m_arrShaderCode.size(); ++i)
			{
				const Json::Value& shaderFileName = shaderFileNames[i];
				if (false == shaderFileName.isString())
				{
					ERROR_MESSAGE("문자열이 들어와야 하는데 문자열 형식이 아닙니다.");
					return eResult::Fail_InValid;
				}

				m_arrShaderCode[i].strKey = shaderFileName.asString();

				//에딧 모드가 아닐 경우에만 로드
				if (false == m_bEditMode && false == m_arrShaderCode[i].strKey.empty())
				{
					eResult result = CreateByCSO((eGSStage)i, m_arrShaderCode[i].strKey);
					if (eResultFail(result))
					{
						ERROR_MESSAGE("쉐이더 코드 생성 실패");
						return result;
					}
				}
			}
		}

		//래스터라이저 상태
		ser[JSON_KEY(m_depthStencilType)] >> m_rasterizerType;
		ser[JSON_KEY(m_depthStencilType)] >> m_depthStencilType;
		ser[JSON_KEY(m_blendType)] >> m_blendType;

		//에디트 모드가 아닐 경우에만 Input Layout 생성.
		if (false == m_bEditMode)
		{
			eResult result = CreateInputLayout();
			if (eResultFail(result))
			{
				ERROR_MESSAGE("INPUT LAYOUT 생성 실패");
				return result;
			}
		}


		return eResult::Success;
	}

	eResult GraphicsShader::CreateShader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize)
	{
		ASSERT(_pByteCode, "ByteCode 주소가 nullptr입니다.");
		ASSERT(_ByteCodeSize, "ByteCode의 사이즈가 0입니다.");

		auto pDevice = GPUManager::Device();
		switch (_stage)
		{
		case eGSStage::Vertex:
		{
			if (FAILED(pDevice->CreateVertexShader(_pByteCode, _ByteCodeSize, nullptr, m_vertexShader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Vertex GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}

		case eGSStage::Hull:
		{
			if (FAILED(pDevice->CreateHullShader(_pByteCode, _ByteCodeSize, nullptr, m_hullShader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Hull GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Domain:
		{
			if (FAILED(pDevice->CreateDomainShader(_pByteCode, _ByteCodeSize, nullptr, m_domainShader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Domain GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Geometry:
		{
			if (FAILED(pDevice->CreateGeometryShader(_pByteCode, _ByteCodeSize, nullptr, m_geometryShader.ReleaseAndGetAddressOf())))
			{
				ERROR_MESSAGE("Geometry GraphicsShader 생성에 실패했습니다.");
				return eResult::Fail_Create;
			}

			break;
		}


		case eGSStage::Pixel:
		{
			if (FAILED(pDevice->CreatePixelShader(_pByteCode, _ByteCodeSize, nullptr, m_pixelShader.ReleaseAndGetAddressOf())))
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
