#include "Engine/Game/Component/Light/Com_Light3D.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"

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
		: iLight(eDimensionType::_3D)
		, m_volumeMesh()
		, m_lightMaterial()
		, m_index()
	{
		RenderManager::RegisterLight(this);
	}

	Com_Light3D::Com_Light3D(const Com_Light3D& _other)
		: iLight(_other)
		, m_index(_other.m_index)
		, m_attribute(_other.m_attribute)
		, m_volumeMesh(_other.m_volumeMesh)
		, m_lightMaterial(_other.m_lightMaterial)
	{
	}

	Com_Light3D::~Com_Light3D()
	{
	}

	eResult Com_Light3D::Serialize_Json(JsonSerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		JsonSerializer ser = *_ser;

		try
		{
			Json::Value& light3d = ser[GetStrKey()];

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

			light3d[JSON_KEY(m_volumeMesh)] << m_volumeMesh->GetStrKey();
			light3d[JSON_KEY(m_lightMaterial)] << m_lightMaterial->GetStrKey();

		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	eResult Com_Light3D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const JsonSerializer& ser = *_ser;

		try
		{
			const Json::Value& light3d = ser[GetStrKey()];

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

			m_volumeMesh = ResourceManager<Mesh>::Load(strKey);
			strKey.clear();

			light3d[JSON_KEY(m_lightMaterial)] >> strKey;
			m_lightMaterial = ResourceManager<Material>::Load(strKey);
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
		
		auto tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
			return;

		float3 position = tr->GetLocalPosition();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);
			
		//Transform 조작은 Update()까지만 가능.
		switch ((eLightType)m_attribute.lightType)
		{
		case ehw::eLightType::Directional:
			break;
		case ehw::eLightType::Point:
			tr->SetLocalScale(float3(m_attribute.radius));
			break;
		case ehw::eLightType::Spot:
			break;
		case ehw::eLightType::END:
			break;
		default:
			break;
		}
	}

	void Com_Light3D::FinalUpdate()
	{
		auto tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
		{
			return;
		}

		switch ((eLightType)m_attribute.lightType)
		{
		case ehw::eLightType::Directional:
		{
			const float3& forward = tr->GetWorldDirection(eDirection::Forward);
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

		float3 position = tr->GetLocalPosition();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);

		RenderManager::PushLightAttribute(m_attribute);
	}


	void Com_Light3D::Render()
	{
		if (nullptr == m_lightMaterial)
		{
			return;
		}

		//Transform
		auto tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
			return;
		tr->BindData();

		//Light
		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::numberOfLight);
		tCB_NumberOfLight data = {};
		data.numberOfLight = (uint)RenderManager::GetLights().size();
		data.indexOfLight = m_index;

		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		m_volumeMesh->BindBuffer();
		m_lightMaterial->BindData();
		m_volumeMesh->Render();
		m_lightMaterial->UnBindData();
	}


	void Com_Light3D::SetLightType(eLightType type)
	{
		m_attribute.lightType = (int)type;
		if (m_attribute.lightType == (int)eLightType::Directional)
		{
			m_volumeMesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::RectMesh);
			m_lightMaterial = ResourceManager<Material>::Find(strKey::defaultRes::material::LightDirMaterial);
		}
		else if (m_attribute.lightType == (int)eLightType::Point)
		{
			m_volumeMesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::SphereMesh);
			m_lightMaterial = ResourceManager<Material>::Find(strKey::defaultRes::material::LightPointMaterial);
		}
		else if (m_attribute.lightType == (int)eLightType::Spot)
		{
			ERROR_MESSAGE("미구현");
		}
	}
	
}
