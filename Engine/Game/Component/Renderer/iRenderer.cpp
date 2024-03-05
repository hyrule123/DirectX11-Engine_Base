#include "Game/Component/Renderer/iRenderer.h"

#include "Game/Component/Transform/Com_Transform.h"
#include "Game/Component/Animator/Com_Animator2D.h"
#include "Game/Component/Animator/Com_Animator3D.h"

#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Game/GameObject.h"
#include "Manager/ResourceManager.h"
#include "Resource/Model3D/Skeleton.h"

#include "Util/Serialize/JsonSerializer.h"


namespace ehw
{
	iRenderer::iRenderer()
		: m_mesh(nullptr)
		, m_materials(1)
		, m_bCullingEnable(true)
	{
	}

	iRenderer::~iRenderer()
	{
	}

	iRenderer::iRenderer(const iRenderer& _other)
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

	eResult iRenderer::Serialize_Json(JsonSerializer* _ser) const
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
			Json::Value& renderer = ser[GetStrKey()];

			//m_mesh
			renderer[JSON_KEY(m_mesh)] << m_mesh->GetStrKey();

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

					materials[i] << m_materials[i].SharedMaterial->GetStrKey();
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

	eResult iRenderer::DeSerialize_Json(const JsonSerializer* _ser)
	{
		SERIALIZER_CHECK_PTR(_ser);

		const JsonSerializer& ser = *_ser;

		try
		{
			const Json::Value& renderer = ser[GetStrKey()];

			//m_mesh
			{
				std::string strKey{};
				renderer[JSON_KEY(m_mesh)] >> strKey;
				m_mesh = ResourceManager<Mesh>::Load(strKey);
			}
			

			//materials
			{
				const Json::Value& materials = renderer[JSON_KEY(m_materials)];
				m_materials.resize(materials.size());

				for (size_t i = 0; i < m_materials.size(); ++i)
				{
					std::string strKey{};
					materials[i] >> strKey;
					m_materials[i].SharedMaterial = ResourceManager<Material>::Load(strKey);
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

	void iRenderer::SetMesh(const std::shared_ptr<Mesh> _mesh)
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

	Material* iRenderer::SetMaterialMode(UINT _idx, eMaterialMode _mode)
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
