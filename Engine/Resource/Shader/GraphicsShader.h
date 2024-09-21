#pragma once
#include "Engine/Resource/Shader/Shader.h"

#include "Engine/GPU/CommonGPU.h"
#include "Engine/Util/Serialize/Serializable.h"

namespace editor
{
	class EditorGraphicsShader;
}

//쉐이더는 ResourceManager에서 관리받는 리소스로도 생성가능(json 파일로 저장 가능)
//혹은 Material을 상속받은 클래스에서 직접 생성하는것도 가능하다.

namespace ehw
{
	struct tShaderCode
	{
		const void* pData;	//엔진에 포함된 바이트코드(할당/해제가 필요없을 경우) 사용
		size_t dataSize;
		ComPtr<ID3DBlob> blob;	//파일로부터 읽어온 바이트코드(할당/해제가 필요할 경우) 사용
		std::string strKey;
	};

	BASE_RESOURCE(GraphicsShader);
	class GraphicsShader final 
		: public Shader
		, public Serializable_Json
	{
		CLASS_NAME(GraphicsShader);
	public:
		GraphicsShader();
		virtual ~GraphicsShader();

		virtual eResult save_to_file(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
		virtual eResult load_from_file(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		eResult compile_from_source_code(eGSStage _stage, const std::filesystem::path& _FullPath, const std::string_view _funcName);
		eResult compile_from_byte_code(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult compile_from_CSO(eGSStage _stage, const std::filesystem::path& _FileName);

		void AddInputLayoutDesc(const D3D11_INPUT_ELEMENT_DESC& _desc);
		void SetInputLayoutDesc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs);
		eResult CreateInputLayout();
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescs() { return m_inputLayoutDescs; }

		ID3D11InputLayout* GetInputLayout() { return m_inputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return m_inputLayout.GetAddressOf(); }

		void SetRSState(eRSType _state);
		void SetDSState(eDSType _state);
		void SetBSState(eBSType _state);

		void bind_shader();
		static void unbind_all_shader();

		//에디터용
		void SetEditMode(bool _bEditMode) { m_bEditMode = _bEditMode; }
		void SetShaderKey(eGSStage _stage, const std::string_view _resource_name) {
			m_arrShaderCode[(int)_stage].strKey = _resource_name;
		}
		const std::string& GetShaderKey(eGSStage _stage) { return m_arrShaderCode[(int)_stage].strKey; }

	private:
		eResult create_shader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputLayoutDescs;
		ComPtr<ID3D11InputLayout> m_inputLayout;

		std::array<tShaderCode, (int)eGSStage::END>  m_arrShaderCode;

		ComPtr<ID3D11VertexShader>		m_vertexShader;
		ComPtr<ID3D11HullShader>		m_hullShader;
		ComPtr<ID3D11DomainShader>		m_domainShader;
		ComPtr<ID3D11GeometryShader>	m_geometryShader;
		ComPtr<ID3D11PixelShader>		m_pixelShader;

		ComPtr<ID3D11RasterizerState>	m_rasterizer;
		ComPtr<ID3D11BlendState>		m_blender;
		ComPtr<ID3D11DepthStencilState> m_depth_stencil;

		eRSType m_rasterizer_type;
		eBSType m_blender_type;
		eDSType m_depth_stencil_type;

		FLOAT m_blend_factor[4];
		UINT m_sample_mask;
		UINT m_stencil_ref;

		ComPtr<ID3DBlob> m_errorBlob;

		bool m_bEditMode;
	};
}
