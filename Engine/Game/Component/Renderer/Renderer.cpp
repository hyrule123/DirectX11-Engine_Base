#include "Engine/Game/Component/Renderer/Renderer.h"

#include "Engine/Game/Component/Com_Transform.h"
#include "Engine/Game/Component/Animator/Com_Animator2D.h"
#include "Engine/Game/Component/Animator/Com_Animator3D.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"
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
		, m_materials(1)
		, m_bCullingEnable(true)
	{
	}

	Renderer::~Renderer()
	{
	}

	Renderer::Renderer(const Renderer& _other)
		: Component(_other)
		, m_mesh(_other.m_mesh)
		, m_materials{}
		, m_bCullingEnable(_other.m_bCullingEnable)
	{
		m_materials.resize(_other.m_materials.size());
		for (size_t i = 0; i < m_materials.size(); ++i)
		{
			//Shared Material은 공유 항목이므로 그대로 복사
			m_materials[i].SharedMaterial = _other.m_materials[i].SharedMaterial;

			//Dynamic Material은 고유 항목이므로 Clone해서 이쪽도 마찬가지로 고유 항목 생성
			if (_other.m_materials[i].DynamicMaterial)
			{
				m_materials[i].DynamicMaterial = std::unique_ptr<Material>(_other.m_materials[i].DynamicMaterial->Clone());
			}
		}
	}

	void Renderer::FinalUpdate()
	{
		RenderManager::GetInst().sceneRenderAgent().EnqueueRenderer(this);
	}

	eResult Renderer::Serialize_Json(JsonSerializer* _ser) const
	{
		SERIALIZER_CHECK_PTR(_ser);

		JsonSerializer& ser = *_ser;

		if (nullptr == m_mesh)
		{
			ERROR_MESSAGE("Mesh 정보가 없습니다.");
			return eResult::Fail_InValid;
		}
		else if (m_materials.empty())
		{
			ERROR_MESSAGE("Material 정보가 없습니다.");
			return eResult::Fail_InValid;
		}


		try
		{
			Json::Value& renderer = ser[get_strkey()];

			//m_mesh
			renderer[JSON_KEY(m_mesh)] << m_mesh->get_strkey();

			//materials
			{
				Json::Value& materials = renderer[JSON_KEY(m_materials)];
				materials.resize((Json::ArrayIndex)m_materials.size());
				
				for (size_t i = 0; i < m_materials.size(); ++i)
				{
					if (nullptr == m_materials[i].SharedMaterial)
					{
						ERROR_MESSAGE("Material 정보가 없습니다.");
						return eResult::Fail_Nullptr;
					}

					materials[i] << m_materials[i].SharedMaterial->get_strkey();
				}
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

	eResult Renderer::DeSerialize_Json(const JsonSerializer* _ser)
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
				m_mesh = ResourceManager<Mesh>::GetInst().Load(strKey);
			}
			

			//materials
			{
				const Json::Value& materials = renderer[JSON_KEY(m_materials)];
				m_materials.resize(materials.size());

				for (size_t i = 0; i < m_materials.size(); ++i)
				{
					std::string strKey{};
					materials[i] >> strKey;
					m_materials[i].SharedMaterial = ResourceManager<Material>::GetInst().Load(strKey);
				}
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

	void Renderer::SetMesh(const std::shared_ptr<Mesh> _mesh)
	{
		m_mesh = _mesh;

		//if (false == m_materials.empty())
		//{
		//	m_materials.clear();
		//	std::vector<tMaterialSet> materials;
		//	m_materials.swap(materials);
		//}

		if (nullptr != m_mesh)
			m_materials.resize(m_mesh->GetSubsetCount());
	}

	void Renderer::SetMaterial(const std::shared_ptr<Material>& _Mtrl, UINT _idx)
	{
		if ((UINT)m_materials.size() <= _idx)
			m_materials.resize(_idx + 1u);

		m_materials[_idx] = {};
		m_materials[_idx].SharedMaterial = _Mtrl;
		m_materials[_idx].CurrentMaterial = m_materials[_idx].SharedMaterial.get();
	}

	Material* Renderer::SetMaterialMode(UINT _idx, eMaterialMode _mode)
	{
		tMaterialSet* mtrlSet = GetMaterialSet(_idx);
		Material* mtrl = nullptr;
		if (mtrlSet)
		{
			if (eMaterialMode::Shared == _mode)
			{
				mtrlSet->CurrentMaterial = mtrlSet->SharedMaterial.get();
				mtrlSet->MaterialMode = _mode;
			}
			else if (eMaterialMode::Dynamic == _mode)
			{
				//Dynamic Material이 없는데 Shared Material이 있을 경우 복사해서 새로 생성
				if (mtrlSet->SharedMaterial)
				{
					if (nullptr == mtrlSet->DynamicMaterial)
					{
						mtrlSet->DynamicMaterial = std::unique_ptr<Material>(mtrlSet->SharedMaterial->Clone());
					}
					mtrlSet->CurrentMaterial = mtrlSet->DynamicMaterial.get();
					mtrlSet->MaterialMode = _mode;
				}
			}
			mtrl = mtrlSet->CurrentMaterial;
		}

		return mtrl;
	}

}
