#include "Engine/Game/Component/Light/Light_3D.h"

#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Light/Light.hlsli"

namespace core
{
	Light_3D::Light_3D()
		: Light(Light_3D::s_concrete_class_name, eDimensionType::_3D)
	{
	}

	Light_3D::Light_3D(const Light_3D& _other)
		: Light(_other)
		, m_attribute(_other.m_attribute)
	{
	}

	Light_3D::~Light_3D()
	{
	}

	eResult Light_3D::serialize_json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer ser = *_ser;

		try
		{
			Json::Value& light3d = ser[get_concrete_class_name()];

			//m_attribute
			{
				Json::Value& attribute = light3d[JSON_KEY(m_attribute)];

				attribute[JSON_KEY(m_attribute.angle)]			<< m_attribute.angle;
				attribute[JSON_KEY(m_attribute.color.ambient)]	<< m_attribute.color.ambient;
				attribute[JSON_KEY(m_attribute.color.diffuse)]	<< m_attribute.color.diffuse;
				attribute[JSON_KEY(m_attribute.color.specular)] << m_attribute.color.specular;
				attribute[JSON_KEY(m_attribute.direction)]		<< m_attribute.direction;
				attribute[JSON_KEY(m_attribute.lightType)]		<< m_attribute.lightType;
				attribute[JSON_KEY(m_attribute.position)]		<< m_attribute.position;
				attribute[JSON_KEY(m_attribute.radius)]			<< m_attribute.radius;
			}

		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	eResult Light_3D::deserialize_json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		try
		{
			const Json::Value& light3d = ser[get_concrete_class_name()];

			//m_attribute
			{
				const Json::Value& attribute = light3d[JSON_KEY(m_attribute)];

				attribute[JSON_KEY(m_attribute.angle)] >> m_attribute.angle;
				attribute[JSON_KEY(m_attribute.color.ambient)] >> m_attribute.color.ambient;
				attribute[JSON_KEY(m_attribute.color.diffuse)] >> m_attribute.color.diffuse;
				attribute[JSON_KEY(m_attribute.color.specular)] >> m_attribute.color.specular;
				attribute[JSON_KEY(m_attribute.direction)] >> m_attribute.direction;
				attribute[JSON_KEY(m_attribute.lightType)] >> m_attribute.lightType;
				attribute[JSON_KEY(m_attribute.position)] >> m_attribute.position;
				attribute[JSON_KEY(m_attribute.radius)] >> m_attribute.radius;
			}
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	//트랜스폼을 업데이트
	void Light_3D::update()
	{
		s_ptr<GameObject> owner = get_owner();
		ASSERT_DEBUG(owner, "owner gameobject가 없습니다.");

		auto tr = owner->GetComponent<Transform>();
		if (nullptr == tr) {
			return;
		}

		float3 position = tr->get_world_position();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);
			
		//Transform 조작은 update()까지만 가능.
		switch (m_attribute.lightType)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			break;
		case LIGHT_TYPE_POINT:
			tr->set_world_scale(float3(m_attribute.radius));
			break;
		case LIGHT_TYPE_SPOT:
			break;
		default:
			ASSERT(false, "Light Type이 지정되지 않았습니다.");
			break;
		}
	}

	//트랜스폼으로부터 데이터를 업데이트
	void Light_3D::final_update()
	{
		s_ptr<GameObject> owner = get_owner();
		ASSERT_DEBUG(owner, "owner GameObject가 없습니다.");

		auto tr = owner->GetComponent<Transform>();
		if (nullptr == tr)
		{
			return;
		}

		switch (m_attribute.lightType)
		{
		case LIGHT_TYPE_DIRECTIONAL:
		{
			const float3& forward = tr->get_world_direction(eDirection::Forward);
			m_attribute.direction = float4(forward, 0.0f);
			break;
		}

		case LIGHT_TYPE_POINT:
			//Update에서 수행했음.
			break;
		case LIGHT_TYPE_SPOT:
			break;
		default:
			ASSERT(false, "Light Type 미지정");
			break;
		}

		float3 position = tr->get_world_position();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);

		RenderManager::get_inst().sceneRenderAgent().enqueue_light_3D((eLightType)m_attribute.lightType, this);
	}
}
