#pragma once
#include <Engine/Resource/Resource.h>

#include <Engine/GPU/CommonGPU.h>
#include <Engine/Util/Serialize/Serializable.h>


namespace core
{
	class InputLayout;
	class VertexShader;
	class HullShader; 
	class DomainShader; 
	class GeometryShader; 
	class PixelShader;
	class RasterizerState;
	class DepthStencilState;
	class BlendState;
	class SamplerState;

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

		void set_input_layout(s_ptr<InputLayout> _layout)
		{ 
			if (_layout) { m_input_layout = std::move(_layout); }
		}
		void set_vertex_shader(s_ptr<VertexShader> _shader) { 
			if (_shader) { m_vertex_shader = std::move(_shader); }
		}
		void set_hull_shader(s_ptr<HullShader> _shader) 
		{ 
			if (_shader) { m_hull_shader = std::move(_shader); }
		}
		void set_domain_shader(s_ptr<DomainShader> _shader) 
		{ 
			if (_shader) { m_domain_shader = std::move(_shader); }
		}
		void set_geometry_shader(s_ptr<GeometryShader> _shader) 
		{ 
			if (_shader) { m_geometry_shader = std::move(_shader); }
		}
		void set_pixel_shader(s_ptr<PixelShader> _shader) 
		{ 
			if (_shader) { m_pixel_shader = std::move(_shader); }
		}
		void set_sampler_state(s_ptr<SamplerState> _state) 
		{ 
			if (_state) { m_sampler_state = std::move(_state); }
		}
		void set_rasterizer_state(s_ptr<RasterizerState> _state) 
		{ 
			if (_state) { m_rasterizer_state = std::move(_state); }
		}
		void set_blend_state(s_ptr<BlendState> _state) 
		{ 
			if (_state) { m_blend_state = std::move(_state); }
		}
		void set_depth_stencil_state(s_ptr<DepthStencilState> _state) 
		{ 
			if (_state) { m_depth_stencil_state = std::move(_state); }
		}


		void bind();
		static void unbind_all();

	private:
		std::shared_ptr<class InputLayout>		m_input_layout = {};
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
