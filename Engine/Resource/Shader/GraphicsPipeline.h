#pragma once
#include <Engine/Resource/Resource.h>

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
	class GraphicsPipeline final 
		: public Resource
	{
		CLASS_INFO(GraphicsPipeline, Resource);
		BASE_RESOURCE(GraphicsPipeline, eResourceType::GraphicsPipeline);

		NO_SERIALIZE_BINARY;

	public:
		GraphicsPipeline();


		virtual ~GraphicsPipeline();

		virtual eResult save(const std::fs::path& _full_path) const override;
		virtual eResult load(const std::fs::path& _full_path) override;

		virtual eResult serialize_json(JsonSerializer& _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer& _ser) override;

		void set_rasterizer_state(eRasterizerState _state) { m_rasterizer_type = _state; };
		void set_depth_stencil_state(eDepthStencilState _state) { m_depth_stencil_type = _state; };
		void set_blend_state(eBlendState _state) { m_blend_type = _state; };
		eRasterizerState get_rasterizer_state() const { return m_rasterizer_type; }
		eDepthStencilState get_depth_stencil_state() const { return m_depth_stencil_type; }
		eBlendState get_blend_state() const { return m_blend_type; }

		void bind_shader();
		static void unbind_all_shader();

	private:
		eResult create_shader(eGSStage _stage, const void* _pByteCode, size_t _ByteCodeSize);

	private:
		std::shared_ptr<class VertexShader>		m_vertex_shader = {};
		std::shared_ptr<class HullShader>		m_hull_shader = {};
		std::shared_ptr<class DomainShader>		m_domain_shader = {};
		std::shared_ptr<class GeometryShader>	m_geometry_shader = {};
		std::shared_ptr<class PixelShader>		m_pixel_shader = {};

		eRasterizerState m_rasterizer_type = {};
		eBlendState m_blend_type = {};
		eDepthStencilState m_depth_stencil_type = {};

		FLOAT m_blend_factor[4] = {};
		UINT m_sample_mask = {};
		UINT m_stencil_ref = {};

		ComPtr<ID3DBlob> m_error_blob = {};

		bool m_b_edit_mode = {};
	};
}
