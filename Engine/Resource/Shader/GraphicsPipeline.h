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

		void set_vertex_shader(const s_ptr<class VertexShader>& _shader) { 
			if (_shader) { m_vertex_shader = _shader; }
		}
		void set_hull_shader(const s_ptr<class HullShader>& _shader) 
		{ 
			if (_shader) { m_hull_shader = _shader; }
		}
		void set_domain_shader(const s_ptr<class DomainShader>& _shader) 
		{ 
			if (_shader) { m_domain_shader = _shader; }
		}
		void set_geometry_shader(const s_ptr<class GeometryShader>& _shader) 
		{ 
			if (_shader) { m_geometry_shader = _shader; }
		}
		void set_pixel_shader(const s_ptr<class PixelShader>& _shader) 
		{ 
			if (_shader) { m_pixel_shader = _shader; }
		}
		void set_sampler_state(const s_ptr<class SamplerState>& _state) 
		{ 
			if (_state) { m_sampler_state = _state; }
		}
		void set_rasterizer_state(const s_ptr<class RasterizerState>& _state) 
		{ 
			if (_state) { m_rasterizer_state = _state; }
		}
		void set_blend_state(const s_ptr<class BlendState>& _state) 
		{ 
			if (_state) { m_blend_state = _state; }
		}
		void set_depth_stencil_state(const s_ptr<class DepthStencilState>& _state) 
		{ 
			if (_state) { m_depth_stencil_state = _state; }
		}


		void bind();
		static void unbind_all();

	private:
		std::shared_ptr<class VertexShader>		m_vertex_shader = {};
		std::shared_ptr<class HullShader>		m_hull_shader = {};
		std::shared_ptr<class DomainShader>		m_domain_shader = {};
		std::shared_ptr<class GeometryShader>	m_geometry_shader = {};
		std::shared_ptr<class PixelShader>		m_pixel_shader = {};

		std::shared_ptr<class SamplerState>		m_sampler_state = {};
		std::shared_ptr<class RasterizerState>	m_rasterizer_state = {};
		std::shared_ptr<class BlendState>		m_blend_state = {};
		std::shared_ptr<class DepthStencilState> m_depth_stencil_state = {};
	};
}
