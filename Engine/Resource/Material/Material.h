#pragma once
#include <Engine/define_Macro.h>
#include <Engine/define_Enum.h>

#include <Engine/GPU/CommonGPU.h>

#include <Engine/Util/Serialize/Serializable.h>

#include <Engine/Resource/Resource.h>

#include <array>


namespace core
{
	class StructBuffer;
	class Texture;
	class GraphicsShader;

	BASE_RESOURCE(Material);
	class Material 
		: public Resource
		, public Serializable_Json
	{
		CLASS_INFO(Material, Resource);
		CLONE_ABLE(Material);
	public:
		Material();

		Material(const Material& _other);

		virtual ~Material();


		// 필수 재정의 함수들 //
		void bind_shader();
		virtual void clear_instancing_buffer() {}; //버퍼 클리어
		virtual void set_data_to_instancing_buffer(const std::vector<s_ptr<GameObject>>& _objs) {};
		virtual void bind_instancing_buffer_to_GPU_register() {};
		///////////////////////////////

		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void set_shader(const s_ptr<GraphicsShader>& _shader) { m_shader = _shader; }
		const s_ptr<GraphicsShader>& get_shader() const { return m_shader; }

		void set_texture(eTextureSlot _slot, const s_ptr<Texture>& _texture);
		const s_ptr<Texture>& get_texture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode get_rendering_mode() const { return m_renderingMode; }
		void set_rendering_mode(eRenderingMode _mode) { m_renderingMode = _mode; }

		void set_diffuse(const float4& _diff) { m_shared_material_data.diffuse = _diff; }
		void set_specular(const float4& _spec) { m_shared_material_data.specular = _spec; }
		void set_amibence(const float4& _amb) { m_shared_material_data.ambient = _amb; }
		void set_emissvie(const float4& _emsv) { m_shared_material_data.emissive = _emsv; }

		float4 get_diffuse_color() const { return m_shared_material_data.diffuse; }
		float4 get_specular_color() const { return m_shared_material_data.specular; }
		float4 get_ambient_color() const { return m_shared_material_data.ambient; }
		float4 get_emmisive_color() const { return m_shared_material_data.emissive; }

		void set_diffuse_color(const float4& _diff)		{ m_shared_material_data.diffuse = _diff; }
		void set_specular_color(const float4& _spec)	{ m_shared_material_data.specular = _spec; }
		void set_ambient_color(const float4& _amb)		{ m_shared_material_data.ambient = _amb; }
		void set_emmisive_color(const float4& _emsv)	{ m_shared_material_data.emissive = _emsv; }
			
	private:
		s_ptr<GraphicsShader> m_shader;
		std::array<s_ptr<Texture>, (int)eTextureSlot::END> m_textures;

		eRenderingMode m_renderingMode;
		tSharedMaterialData m_shared_material_data;
	};
}

