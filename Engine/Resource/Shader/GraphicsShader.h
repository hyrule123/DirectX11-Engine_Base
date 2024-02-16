#pragma once
#include "iShader.h"



#include "../../GPU/CommonGPU.h"
#include "../../Util/Serialize/Serializable.h"


#include <unordered_set>
#include <array>

namespace editor
{
	class EditorGraphicsShader;
}

namespace ehw
{
	struct tShaderCode
	{
		ComPtr<ID3DBlob> blob;
		std::string strKey;
	};

	class GraphicsShader final 
		: public iShader
		, public Serializable_Json
	{
		friend class editor::EditorGraphicsShader;
	public:
		GraphicsShader();
		virtual ~GraphicsShader();

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

		virtual eResult Serialize_Json(JsonSerializer* _ser) override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		eResult CreateByCompile(eGSStage _stage, const std::filesystem::path& _FullPath, const std::string_view _funcName);
		eResult CreateByHeader(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult CreateByCSO(eGSStage _stage, const std::filesystem::path& _FileName);

		inline void AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc);
		inline void SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs);
		eResult CreateInputLayout();
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescs() { return m_inputLayoutDescs; }

		ID3D11InputLayout* GetInputLayout() { return m_inputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return m_inputLayout.GetAddressOf(); }

		void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology) { m_topology = _topology; }
		D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_topology; }

		void SetRSState(eRSType _state) { m_rasterizerType = _state; }
		eRSType GetRSState() const { return m_rasterizerType; }

		void SetDSState(eDSType _state) { m_depthStencilType = _state; }
		eDSType GetDSState() const { return m_depthStencilType; }

		void SetBSState(eBSType _state) { m_blendType = _state; }
		eBSType GetBSState() const { return m_blendType; }

		void BindData();

		//에디터용
		inline void SetEditMode(bool _bEditMode) { m_bEditMode = _bEditMode; }
		inline void SetShaderKey(eGSStage _stage, const std::string_view _strKey);
		inline const std::string& GetShaderKey(eGSStage _stage) { return m_arrShaderCode[(int)_stage].strKey; }
		

	private:
		eResult CreateShader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputLayoutDescs;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		D3D11_PRIMITIVE_TOPOLOGY m_topology;

		std::array<tShaderCode, (int)eGSStage::END>  m_arrShaderCode;

		ComPtr<ID3D11VertexShader>		m_vertexShader;
		ComPtr<ID3D11HullShader>		m_hullShader;
		ComPtr<ID3D11DomainShader>		m_domainShader;
		ComPtr<ID3D11GeometryShader>	m_geometryShader;
		ComPtr<ID3D11PixelShader>		m_pixelShader;

		eRSType m_rasterizerType;
		eDSType m_depthStencilType;
		eBSType m_blendType;

		ComPtr<ID3DBlob> m_errorBlob;

		bool m_bEditMode;
	};

	inline void GraphicsShader::AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc)
	{
		m_inputLayoutDescs.push_back(_desc);

		//이름을 별도 저장된 공간에 저장된 뒤 해당 주소의 포인터로 교체
		const auto& pair = g_cstrArchive.insert(m_inputLayoutDescs.back().SemanticName);
		m_inputLayoutDescs.back().SemanticName = pair.first->c_str();
	}

	inline void GraphicsShader::SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs)
	{
		m_inputLayoutDescs = _descs;

		for (size_t i = 0; i < m_inputLayoutDescs.size(); ++i)
		{
			if (m_inputLayoutDescs[i].SemanticName)
			{
				const auto& pair = g_cstrArchive.insert(m_inputLayoutDescs[i].SemanticName);
				m_inputLayoutDescs[i].SemanticName = pair.first->c_str();
			}
		}
	}


	inline void GraphicsShader::SetShaderKey(eGSStage _stage, const std::string_view _strKey)
	{
		m_arrShaderCode[(int)_stage].strKey = _strKey;
	}
}
