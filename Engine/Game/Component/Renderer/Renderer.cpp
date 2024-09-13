#include "Engine/Game/Component/Renderer/Renderer.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Animator/Com_Animator2D.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"
#include "Engine/Game/GameObject.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Resource/Model3D/Skeleton.h"

#include "Engine/Util/Serialize/JsonSerializer.h"

#include "Engine/Manager/RenderManager.h"


namespace ehw
{
	Renderer::Renderer(const std::string_view key)
		: Component(key)
		, m_mesh(nullptr)
		, m_shared_material(nullptr)
		, m_dynamic_material(nullptr)
		, m_current_material(nullptr)
		, m_material_mode(eMaterialMode::Shared)
		, m_bCullingEnable(true)
	{
	}

	Renderer::~Renderer()
	{
	}

	Renderer::Renderer(const Renderer& _other)
		: Component(_other)
		, m_mesh(_other.m_mesh)
		, m_shared_material()
		, m_dynamic_material(nullptr)
		, m_current_material(nullptr)
		, m_material_mode(_other.m_material_mode)
		, m_bCullingEnable(_other.m_bCullingEnable)
	{
		//Shared Material은 공유 항목이므로 그대로 복사
		m_shared_material = _other.m_shared_material;

		//Dynamic Material은 고유 항목이므로 Clone해서 이쪽도 마찬가지로 고유 항목 생성
		if (_other.m_dynamic_material)
		{
			m_dynamic_material = std::unique_ptr<Material>(static_cast<Material*>(_other.m_dynamic_material->Clone()));
		}
	}

	void Renderer::final_update()
	{
		RenderManager::GetInst().sceneRenderAgent().enqueue_render(this);
	}

	eResult Renderer::serialize_json(JsonSerializer* _ser) const
	{
		SERIALIZER_CHECK_PTR(_ser);

		JsonSerializer& ser = *_ser;

		if (nullptr == m_mesh)
		{
			ERROR_MESSAGE("Mesh 정보가 없습니다.");
			return eResult::Fail_InValid;
		}
		else if (nullptr == m_shared_material)
		{
			ERROR_MESSAGE("Material 정보가 없습니다.");
			return eResult::Fail_InValid;
		}

		try
		{
			Json::Value& renderer = ser[get_strkey()];

			//m_mesh
			renderer[JSON_KEY(m_mesh)] << m_mesh->get_keypath();

			//materials
			{
				Json::Value& materials = renderer[JSON_KEY(m_materials)];
				materials << m_shared_material->get_keypath();
			}

			//m_bCullingEnable
			renderer[JSON_KEY(m_bCullingEnable)] << m_bCullingEnable;

		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult::Success;
	}

	eResult Renderer::deserialize_json(const JsonSerializer* _ser)
	{
		SERIALIZER_CHECK_PTR(_ser);

		const JsonSerializer& ser = *_ser;

		try
		{
			const Json::Value& renderer = ser[get_strkey()];

			//m_mesh
			{
				std::string strKey{};
				renderer[JSON_KEY(m_mesh)] >> strKey;
				m_mesh = ResourceManager<Mesh>::GetInst().load(strKey);
			}
			

			//material
			{
				const Json::Value& material = renderer[JSON_KEY(m_material)];
				std::string strKey{};
				material >> strKey;
				m_shared_material = ResourceManager<Material>::GetInst().load(strKey);
				m_current_material = m_shared_material;
			}

			//m_bCullingEnable
			renderer[JSON_KEY(m_bCullingEnable)] >> m_bCullingEnable;
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_InValid;
		}

		return eResult();
	}

	void Renderer::SetMaterial(const std::shared_ptr<Material>& _mtrl)
	{
		m_shared_material = _mtrl;
		m_current_material = m_shared_material;
	}

	std::shared_ptr<Material> Renderer::SetMaterialMode(eMaterialMode _mode)
	{
		std::shared_ptr<Material> mtrl = nullptr;
		if (m_shared_material)
		{
			if (eMaterialMode::Shared == _mode)
			{
				m_current_material = m_shared_material;
			}
			else if (eMaterialMode::Dynamic == _mode)
			{
				if (nullptr == m_dynamic_material) {
					auto* clone = m_shared_material->Clone();
					ASSERT(clone, "복제 실패");
					if (clone) {
						m_dynamic_material =
							std::dynamic_pointer_cast<Material>(clone->shared_from_this());
					}
				}
				m_current_material = m_dynamic_material;
			}
			m_material_mode = _mode;
			mtrl = m_current_material;
		}

		return mtrl;
	}

}
