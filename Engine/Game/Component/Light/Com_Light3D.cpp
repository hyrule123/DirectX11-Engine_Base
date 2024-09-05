#include "Engine/Game/Component/Light/Com_Light3D.h"

#include "Engine/Game/Component/Transform.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include "Engine/GPU/ConstBuffer.h"

namespace ehw
{
	Com_Light3D::Com_Light3D()
		: Light(Com_Light3D::concrete_name, eDimensionType::_3D)
		, m_volumeMesh()
		, m_lightMaterial()
		, m_index()
	{
	}

	Com_Light3D::Com_Light3D(const Com_Light3D& _other)
		: Light(_other)
		, m_index(_other.m_index)
		, m_attribute(_other.m_attribute)
		, m_volumeMesh(_other.m_volumeMesh)
		, m_lightMaterial(_other.m_lightMaterial)
	{
	}

	Com_Light3D::~Com_Light3D()
	{
	}

	eResult Com_Light3D::serialize_json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer ser = *_ser;

		try
		{
			Json::Value& light3d = ser[get_strkey()];

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

			//m_volumeMesh
			if (nullptr == m_volumeMesh)
			{
				ERROR_MESSAGE("Volume Mesh 정보가 없습니다.");
				return eResult::Fail_InValid;
			}
			else if (nullptr == m_lightMaterial)
			{
				ERROR_MESSAGE("Light Material 정보가 없습니다.");
				return eResult::Fail_InValid;
			}

			light3d[JSON_KEY(m_volumeMesh)] << m_volumeMesh->get_strkey();
			light3d[JSON_KEY(m_lightMaterial)] << m_lightMaterial->get_strkey();

		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	eResult Com_Light3D::deserialize_json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		try
		{
			const Json::Value& light3d = ser[get_strkey()];

			//m_attribute
			{
				const Json::Value& attribute = light3d[JSON_KEY(m_attribute)];

				attribute[JSON_KEY(m_attribute.angle)]				>> m_attribute.angle;
				attribute[JSON_KEY(m_attribute.color.ambient)]		>> m_attribute.color.ambient;
				attribute[JSON_KEY(m_attribute.color.diffuse)]		>> m_attribute.color.diffuse;
				attribute[JSON_KEY(m_attribute.color.specular)]		>> m_attribute.color.specular;
				attribute[JSON_KEY(m_attribute.direction)]			>> m_attribute.direction;
				attribute[JSON_KEY(m_attribute.lightType)]			>> m_attribute.lightType;
				attribute[JSON_KEY(m_attribute.position)]			>> m_attribute.position;
				attribute[JSON_KEY(m_attribute.radius)]				>> m_attribute.radius;
			}

			//m_volumeMesh
			if (nullptr == m_volumeMesh)
			{
				ERROR_MESSAGE("Volume Mesh 정보가 없습니다.");
				return eResult::Fail_InValid;
			}
			else if (nullptr == m_lightMaterial)
			{
				ERROR_MESSAGE("Light Material 정보가 없습니다.");
				return eResult::Fail_InValid;
			}

			std::string strKey{};
			light3d[JSON_KEY(m_volumeMesh)] >> strKey;

			m_volumeMesh = ResourceManager<Mesh>::GetInst().Load(strKey);
			strKey.clear();

			light3d[JSON_KEY(m_lightMaterial)] >> strKey;
			m_lightMaterial = ResourceManager<Material>::GetInst().Load(strKey);
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}


	void Com_Light3D::Update()
	{
		
		auto tr = gameObject()->GetComponent<Transform>();
		if (nullptr == tr)
			return;

		float3 position = tr->get_local_position();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);
			
		//Transform 조작은 Update()까지만 가능.
		switch ((eLightType)m_attribute.lightType)
		{
		case ehw::eLightType::Directional:
			break;
		case ehw::eLightType::Point:
			tr->set_local_scale(float3(m_attribute.radius));
			break;
		case ehw::eLightType::Spot:
			break;
		case ehw::eLightType::END:
			break;
		default:
			break;
		}
	}

	void Com_Light3D::final_update()
	{
		auto tr = gameObject()->GetComponent<Transform>();
		if (nullptr == tr)
		{
			return;
		}

		switch ((eLightType)m_attribute.lightType)
		{
		case ehw::eLightType::Directional:
		{
			const float3& forward = tr->get_world_direction(eDirection::Forward);
			m_attribute.direction = float4(forward, 0.0f);
			
			break;
		}

		case ehw::eLightType::Point:
			//Update에서 수행했음.
			break;
		case ehw::eLightType::Spot:
			break;
		case ehw::eLightType::END:
			break;
		default:
			break;
		}

		float3 position = tr->get_local_position();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);

		RenderManager::GetInst().sceneRenderAgent().RegisterLight(this);
		RenderManager::GetInst().sceneRenderAgent().PushLightAttribute(m_attribute);
	}


	void Com_Light3D::Render()
	{
		if (nullptr == m_lightMaterial)
		{
			return;
		}

		//Transform
		auto tr = gameObject()->GetComponent<Transform>();
		if (nullptr == tr)
			return;
		tr->bind_data();

		//Light
		ConstBuffer* cb = RenderManager::GetInst().GetConstBuffer(eCBType::numberOfLight);
		tCB_NumberOfLight data = {};
		data.numberOfLight = (uint)RenderManager::GetInst().sceneRenderAgent().GetLights().size();
		data.indexOfLight = m_index;

		cb->SetData(&data);
		cb->bind_data(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		//m_volumeMesh->BindBuffer();
		m_lightMaterial->bind_data();
		m_volumeMesh->Render();
		//m_lightMaterial->UnbindData();
	}


	void Com_Light3D::SetLightType(eLightType type)
	{
		m_attribute.lightType = (int)type;
		if (m_attribute.lightType == (int)eLightType::Directional)
		{
			m_volumeMesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::RectMesh);
			m_lightMaterial = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::LightDirMaterial);
		}
		else if (m_attribute.lightType == (int)eLightType::Point)
		{
			m_volumeMesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::SphereMesh);
			m_lightMaterial = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::LightPointMaterial);
		}
		else if (m_attribute.lightType == (int)eLightType::Spot)
		{
			ERROR_MESSAGE("미구현");
		}
	}
	
}
