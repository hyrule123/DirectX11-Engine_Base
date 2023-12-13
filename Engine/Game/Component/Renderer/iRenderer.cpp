
#include "iRenderer.h"



#include "../../../Resource/Mesh.h"
#include "../../../Resource/Material.h"
#include "../../../Game/GameObject.h"
#include "../../../json-cpp/json.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Resource/Modeling/Skeleton.h"


#include "../Transform/Com_Transform.h"
#include "../Animator/Com_Animator2D.h"
#include "../Animator/Com_Animator3D.h"

namespace ehw
{
	iRenderer::iRenderer()
		: iComponent(eComponentCategory::Renderer)
		, mMesh(nullptr)
		, mMaterials(1)
		, mbCullingEnable(true)
	{
	}

	iRenderer::~iRenderer()
	{
	}

	iRenderer::iRenderer(const iRenderer& _other)
		: iComponent(_other)
		, mMesh(_other.mMesh)
		, mMaterials{}
		, mbCullingEnable(_other.mbCullingEnable)
	{
		mMaterials.resize(_other.mMaterials.size());
		for (size_t i = 0; i < mMaterials.size(); ++i)
		{
			//Shared Material은 공유 항목이므로 그대로 복사
			mMaterials[i].SharedMaterial = _other.mMaterials[i].SharedMaterial;

			//Dynamic Material은 고유 항목이므로 Clone해서 이쪽도 마찬가지로 고유 항목 생성
			if (_other.mMaterials[i].DynamicMaterial)
			{
				mMaterials[i].DynamicMaterial = std::unique_ptr<Material>(_other.mMaterials[i].DynamicMaterial->Clone());
			}
		}
	}

	eResult iRenderer::SaveJson(Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = iComponent::SaveJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		Json::Value& jVal = *_pJson;

		//포인터의 경우 키값을 저장
		if (mMesh)
		{
			jVal[JSON_KEY(mMesh)] = mMesh->GetStrKey();
		}

		//TODO: FBX 로드를 위해 주석 처리
		//if (mMaterial)
		//{
		//	jVal[JSON_KEY(mMaterial)] = mMaterial->GetKey();
		//}

		return eResult::Success;
	}

	eResult iRenderer::LoadJson(const Json::Value* _pJson)
	{
		if (nullptr == _pJson)
		{
			return eResult::Fail_Nullptr;
		}
		eResult result = iComponent::LoadJson(_pJson);
		if (eResultFail(result))
		{
			return result;
		}
		const Json::Value& jVal = *_pJson;

		if (jVal.isMember(JSON_KEY(mMesh)))
		{
			mMesh = ResourceManager::Load<Mesh>(jVal[JSON_KEY(mMesh)].asString());
		}

		//TODO: FBX 로드를 위해 주석 처리
		//if (jVal.isMember(JSON_KEY(mMaterial)))
		//{
		//	mMaterial = ResourceManager::Load<Material>(jVal[JSON_KEY(mMaterial)].asString());
		//}

		return eResult::Success;
	}



	void iRenderer::SetMesh(const std::shared_ptr<Mesh> _mesh)
	{
		mMesh = _mesh;

		//if (false == mMaterials.empty())
		//{
		//	mMaterials.clear();
		//	std::vector<tMaterialSet> materials;
		//	mMaterials.swap(materials);
		//}

		if (nullptr != mMesh)
			mMaterials.resize(mMesh->GetSubsetCount());
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
