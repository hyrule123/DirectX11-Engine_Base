#pragma once
#include <Engine/Game/Component/Light/Light.h>
#include <Engine/GPU/CommonGPU.h>

namespace core
{
	class StructBuffer;
	class ConstBuffer;
	class Mesh;
	class Material;
	class Light3D : public Light
	{
		CLASS_INFO(Light3D, Light);
		REGISTER_FACTORY(Light3D);
		CLONE_ABLE(Light3D);

		NO_SERIALIZE_BINARY;

	public:
		Light3D();

		Light3D(const Light3D& _other);
		

		~Light3D() override;

		eResult serialize_json(JsonSerializer& _ser) const override;
		eResult deserialize_json(const JsonSerializer& _ser) override;

		void update() override;
		void final_update() override;
		
		const tLightAttribute& get_light_3D_attribute() const { return m_attribute; }

		void set_diffuse(const float4& diffuse) { m_attribute.color.diffuse = diffuse; }
		void set_specular(const float4& spec) { m_attribute.color.specular = spec; }
		void set_ambient(const float4& ambient) { m_attribute.color.ambient = ambient; }
		void set_light_type(eLightType _type) { m_attribute.lightType = (int)_type; }
		void set_radius(float radius) { m_attribute.radius = radius; }
		void set_angle(float angle) { m_attribute.angle = angle; }
		const float4& get_diffuse() { m_attribute.color.diffuse; }
		float get_radius() { m_attribute.radius; }
		float get_angle() { m_attribute.angle; }

	private:
		tLightAttribute m_attribute;
	};

}
