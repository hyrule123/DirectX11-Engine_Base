#pragma once
#include "Engine/Game/Component/Light/iLight.h"
#include "Engine/GPU/CommonGPU.h"

namespace ehw
{
	class Mesh;
	class Material;
	class Com_Light3D : public iLight
	{
	public:
		Com_Light3D();

		Com_Light3D(const Com_Light3D& _other);
		CLONE_ABLE(Com_Light3D);

		virtual ~Com_Light3D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void Update() override;
		virtual void FinalUpdate() override;

		//Camera가 호출
		virtual void Render() override;

		const tLightAttribute& GetLightAttribute() { return m_attribute; }
		
		void SetDiffuse(const float4& diffuse) { m_attribute.color.diffuse = diffuse; }
		void SetSpecular(const float4& spec) { m_attribute.color.specular = spec; }
		void SetAmbient(const float4& ambient) { m_attribute.color.ambient = ambient; }
		void SetLightType(eLightType type);
		void SetRadius(float radius) { m_attribute.radius = radius; }
		void SetAngle(float angle) { m_attribute.angle = angle; }
		const float4& GetDiffuse() { m_attribute.color.diffuse; }
		eLightType GetType() { return (eLightType)m_attribute.lightType; }
		float GetRadius() { m_attribute.radius; }
		float GetAngle() { m_attribute.angle; }
		void SetIndex(uint _idx) { m_index = _idx; }

	private:
		tLightAttribute m_attribute;
		std::shared_ptr<Mesh> m_volumeMesh; // 광원의 범위를 표현하는 메쉬
		std::shared_ptr<Material> m_lightMaterial;
		uint m_index;
	};

}
