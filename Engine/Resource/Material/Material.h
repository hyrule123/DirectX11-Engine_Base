#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

#include "Engine/GPU/CommonGPU.h"

#include "Engine/Util/Serialize/Serializable.h"

#include "Engine/Resource/Resource.h"

#include <array>


namespace ehw
{
	class StructBuffer;
	class Texture;
	class GraphicsShader;

	BASE_RESOURCE(Material);
	class Material 
		: public Resource
		, public Serializable_Json
	{
		CLASS_NAME(Material);
	public:
		Material(std::string_view key);

		Material(const Material& _other);

		virtual ~Material();

		//RenderManager::CreateMaterial
		static void init_static();
		static void release_static();

		//1, graphics shader, texture
		void bind_shared_material_buffer();
		//2
		void set_data_to_buffer(const std::vector<GameObject*>& _objs);

		// 필수 재정의 함수들 //
		virtual void clear_buffers(); //버퍼 클리어
	protected:
		//반드시 부모 클래스의 해당 함수도 타고올라가면서 호출할것
		virtual void add_data_to_buffer(GameObject* _obj);
	public:
		virtual void bind_buffer_to_GPU();
		///////////////////////////////

		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _key_path) const override;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _key_path) override;

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		void set_shader(const std::shared_ptr<GraphicsShader>& _shader) { m_shader = _shader; }
		const std::shared_ptr<GraphicsShader>& get_shader() const { return m_shader; }

		void set_texture(eTextureSlot _slot, const std::shared_ptr<Texture>& _texture);
		const std::shared_ptr<Texture>& get_texture(eTextureSlot _slot) const { return m_textures[(uint)_slot]; }

		eRenderingMode get_rendering_mode() const { return m_renderingMode; }
		void set_rendering_mode(eRenderingMode _mode) { m_renderingMode = _mode; }

		void set_diffuse(const float4& _diff) { m_shared_material_data.Diff = _diff; }
		void set_specular(const float4& _spec) { m_shared_material_data.Spec = _spec; }
		void set_amibence(const float4& _amb) { m_shared_material_data.Amb = _amb; }
		void set_emissvie(const float4& _emsv) { m_shared_material_data.Emv = _emsv; }

		float4 GetDiffuseColor() const { return m_shared_material_data.Diff; }
		float4 GetSpecularColor() const { return m_shared_material_data.Spec; }
		float4 GetAmbientColor() const { return m_shared_material_data.Amb; }
		float4 GetEmissiveColor() const { return m_shared_material_data.Emv; }

		void SetDiffuseColor(const float4& _diff)		{ m_shared_material_data.Diff = _diff; }
		void SetSpecularColor(const float4& _spec)	{ m_shared_material_data.Spec = _spec; }
		void SetAmbientColor(const float4& _amb)		{ m_shared_material_data.Amb = _amb; }
		void SetEmissiveColor(const float4& _emsv)	{ m_shared_material_data.Emv = _emsv; }
			
	private:
		std::shared_ptr<GraphicsShader> m_shader;
		std::array<std::shared_ptr<Texture>, (int)eTextureSlot::END> m_textures;

		eRenderingMode m_renderingMode;
		tSharedMaterialData m_shared_material_data;

		//이 material을 사용하는 각 GameObject가 가지고 있는 정보
		std::vector<tIndividual_Material_Data> m_individual_mtrl_data;
		static inline StructBuffer* s_individual_mtrl_buffer = nullptr;
	};
}

