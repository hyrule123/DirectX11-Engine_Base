#pragma once
#include "Engine/Game/Component/Light/Light.h"
#include "Engine/GPU/CommonGPU.h"

namespace ehw
{
	class StructBuffer;
	class ConstBuffer;
	class Mesh;
	class Material;
	class Light_3D : public Light
	{
		CLASS_NAME(Light_3D);
		REGISTER_INSTANCE_DEFAULT(Light_3D);
		
	public:
		Light_3D();

		Light_3D(const Light_3D& _other);
		CLONE_ABLE(Light_3D);

		virtual ~Light_3D();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		virtual void Update() override;
		virtual void final_update() override;

		//from RenderManager
		static void init_static();
		static void clear_buffer_data();
		static void release_static();

		//from Camera
		static const std::vector<Light_3D*>& get_lights_by_type(int _light_type) { 
			return s_lights[_light_type];
		}
		void add_to_buffer() { s_buffer_data[m_attribute.lightType].push_back(m_attribute); }
		static void render_lights(int _light_type);
		
		void SetDiffuse(const float4& diffuse) { m_attribute.color.diffuse = diffuse; }
		void SetSpecular(const float4& spec) { m_attribute.color.specular = spec; }
		void SetAmbient(const float4& ambient) { m_attribute.color.ambient = ambient; }
		void SetLightType(eLightType _type) { m_attribute.lightType = (int)_type; }
		void SetRadius(float radius) { m_attribute.radius = radius; }
		void SetAngle(float angle) { m_attribute.angle = angle; }
		const float4& GetDiffuse() { m_attribute.color.diffuse; }
		float GetRadius() { m_attribute.radius; }
		float GetAngle() { m_attribute.angle; }

	private:
		static inline std::shared_ptr<Mesh> s_volume_meshes[LIGHT_TYPE_MAX]{};
		static inline std::shared_ptr<Material> s_light_materials[LIGHT_TYPE_MAX]{};

		static inline std::vector<Light_3D*> s_lights[LIGHT_TYPE_MAX]{};

		static inline std::vector<tLightAttribute> s_buffer_data[LIGHT_TYPE_MAX];
		static inline StructBuffer* s_struct_buffer{};
		static inline ConstBuffer* s_const_buffer{};

		tLightAttribute m_attribute;
	};

}
