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

namespace ehw
{
	Light_3D::Light_3D()
		: Light(Light_3D::concrete_class_name, eDimensionType::_3D)
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
	void Light_3D::Update()
	{
		auto tr = gameObject()->GetComponent<Transform>();
		if (nullptr == tr) {
			return;
		}

		float3 position = tr->get_local_position();
		m_attribute.position = float4(position.x, position.y, position.z, 1.0f);
			
		//Transform 조작은 Update()까지만 가능.
		switch (m_attribute.lightType)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			break;
		case LIGHT_TYPE_POINT:
			tr->set_local_scale(float3(m_attribute.radius));
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
		auto tr = gameObject()->GetComponent<Transform>();
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

		s_lights[m_attribute.lightType].push_back(this);
	}

	void Light_3D::init_static()
	{
		s_volume_meshes[LIGHT_TYPE_DIRECTIONAL] = ResourceManager<Mesh>::GetInst().load(strKey::defaultRes::mesh::RectMesh);
		s_light_materials[LIGHT_TYPE_DIRECTIONAL] = ResourceManager<Material>::GetInst().load(strKey::defaultRes::material::LightDirMaterial);

		s_volume_meshes[LIGHT_TYPE_POINT] = ResourceManager<Mesh>::GetInst().load(strKey::defaultRes::mesh::SphereMesh);
		s_light_materials[LIGHT_TYPE_POINT] = ResourceManager<Material>::GetInst().load(strKey::defaultRes::material::LightPointMaterial);

		SAFE_DELETE(s_struct_buffer);
		s_struct_buffer = new StructBuffer;
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_light_attributes;
		desc.GPU_register_u_UAV = GPU::Register::u::NONE;
		s_struct_buffer->Init<tLightAttribute>(desc, 16);
		s_struct_buffer->SetPipelineTarget(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		SAFE_DELETE(s_const_buffer);
		s_const_buffer = new ConstBuffer(GPU::Register::b::g_CB_light_count);
		s_const_buffer->create<tLightCount>();
		s_const_buffer->SetPresetTargetStage(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
	}

	void Light_3D::clear_buffer_data()
	{
		for (int i = 0; i < LIGHT_TYPE_MAX; ++i) {
			s_lights[i].clear();
			s_buffer_data[i].clear();
		}
	}

	void Light_3D::render_lights(int _light_type)
	{
		tLightCount count;
		count.count = (uint)s_buffer_data[_light_type].size();

		s_const_buffer->SetData(&count);
		s_const_buffer->bind_buffer_to_GPU_register();

		s_struct_buffer->SetData(s_buffer_data[_light_type].data(), s_buffer_data[_light_type].size());
		s_light_materials[_light_type]->bind_buffer_to_gpu_register();
		s_volume_meshes[_light_type]->render_instanced_all((UINT)s_buffer_data[_light_type].size());
	}

	void Light_3D::release_static()
	{
		for (auto& mesh : s_volume_meshes) {
			mesh = nullptr;
		}
		for (auto& mtrl : s_light_materials) {
			mtrl = nullptr;
		}
		
		SAFE_DELETE(s_struct_buffer);
		SAFE_DELETE(s_const_buffer);
	}
}
