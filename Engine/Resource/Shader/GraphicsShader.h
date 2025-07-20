#pragma once
#include <Engine/Resource/Shader/Shader.h>

#include <Engine/GPU/CommonGPU.h>
#include <Engine/Util/Serialize/Serializable.h>

namespace editor
{
	class EditorGraphicsShader;
}

//쉐이더는 ResourceManager에서 관리받는 리소스로도 생성가능(json 파일로 저장 가능)
//혹은 Material을 상속받은 클래스에서 직접 생성하는것도 가능하다.

namespace core
{
	struct tShaderCode
	{
		const void* pData;	//엔진에 포함된 바이트코드(할당/해제가 필요없을 경우) 사용
		size_t dataSize;
		ComPtr<ID3DBlob> blob;	//파일로부터 읽어온 바이트코드(할당/해제가 필요할 경우) 사용
		std::string name;
	};

	BASE_RESOURCE(GraphicsShader);
	class GraphicsShader final 
		: public Shader
		, public Serializable_Json
	{
		CLASS_INFO(GraphicsShader, Shader);
	public:
		GraphicsShader();
		virtual ~GraphicsShader();

		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		eResult compile_from_source_code(eGSStage _stage, const std::filesystem::path& _FullPath, const std::string_view _funcName);
		eResult compile_from_byte_code(eGSStage _stage, const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult compile_from_CSO(eGSStage _stage, const std::filesystem::path& _FileName);

		void add_input_layout_desc(const D3D11_INPUT_ELEMENT_DESC& _desc);
		void set_input_layout_desc(const std::vector<D3D11_INPUT_ELEMENT_DESC>& _descs);
		eResult create_input_layout();
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputLayoutDescs() { return m_input_layout_descs; }

		ID3D11InputLayout* get_input_layout() { return m_input_layout.Get(); }
		ID3D11InputLayout** get_input_layout_pp() { return m_input_layout.GetAddressOf(); }

		void set_rasterizer_state(eRasterizerState _state);
		void set_depth_stencil_state(eDepthStencilState _state);
		void set_blend_state(eBlendState _state);
		eRasterizerState get_rasterizer_state() const { return m_rasterizer_type; }
		eDepthStencilState get_depth_stencil_state() const { return m_depth_stencil_type; }
		eBlendState get_blend_state() const { return m_blend_type; }

		void bind_shader();
		static void unbind_all_shader();

		//에디터용
		void set_edit_mode(bool _bEditMode) { m_b_edit_mode = _bEditMode; }
		void set_shader_name(eGSStage _stage, const std::string_view _resource_name) {
			m_shader_codes[(int)_stage].name = _resource_name;
		}
		const std::string& get_shader_name(eGSStage _stage) { return m_shader_codes[(int)_stage].name; }

	private:
		eResult create_shader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_input_layout_descs;
		ComPtr<ID3D11InputLayout> m_input_layout;

		std::array<tShaderCode, (int)eGSStage::END>  m_shader_codes;

		ComPtr<ID3D11VertexShader>		m_vertex_shader;
		ComPtr<ID3D11HullShader>		m_hull_shader;
		ComPtr<ID3D11DomainShader>		m_domain_shader;
		ComPtr<ID3D11GeometryShader>	m_geometry_shader;
		ComPtr<ID3D11PixelShader>		m_pixel_shader;

		ComPtr<ID3D11RasterizerState>	m_rasterizer_state;
		ComPtr<ID3D11BlendState>		m_blend_state;
		ComPtr<ID3D11DepthStencilState> m_depth_stencil_state;

		eRasterizerState m_rasterizer_type;
		eBlendState m_blend_type;
		eDepthStencilState m_depth_stencil_type;

		FLOAT m_blend_factor[4];
		UINT m_sample_mask;
		UINT m_stencil_ref;

		ComPtr<ID3DBlob> m_error_blob;

		bool m_b_edit_mode;
	};
}
