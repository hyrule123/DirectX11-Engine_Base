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

		~Light_3D() override;

		eResult serialize_json(JsonSerializer* _ser) const override;
		eResult deserialize_json(const JsonSerializer* _ser) override;

		void Update() override;
		void final_update() override;
		
		const tLightAttribute& get_light_3D_attribute() const { return m_attribute; }

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
		tLightAttribute m_attribute;
	};

}
