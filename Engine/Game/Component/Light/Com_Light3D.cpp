#include "Com_Light3D.h"



#include "../Transform/Com_Transform.h"


#include "../../../Manager/RenderManager.h"
#include "../../../Manager/ResourceManager.h"

#include "../../../Resource/Mesh.h"
#include "../../../Resource/Material.h"

#include "../../../Game/GameObject.h"

#include "../../../GPU/ConstBuffer.h"

#include "../../../json-cpp/json.h"

namespace ehw
{
	Com_Light3D::Com_Light3D()
		: iLight(eDimensionType::_3D)
		, mVolumeMesh()
		, mLightMaterial()
		, mIndex()
	{
		RenderManager::AddLight(this);

	}

	Com_Light3D::Com_Light3D(const Com_Light3D& _other)
		: iLight(_other)
		, mIndex(_other.mIndex)
		, mAttribute(_other.mAttribute)
		, mVolumeMesh(_other.mVolumeMesh)
		, mLightMaterial(_other.mLightMaterial)
	{
		RenderManager::AddLight(this);
	}

	Com_Light3D::~Com_Light3D()
	{
		RenderManager::RemoveLight(this);
	}

	eResult Com_Light3D::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = iComponent::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;

		Json::SaveLoad::SaveValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		return eResult::Success;
	}

	eResult Com_Light3D::LoadJson(const Json::Value* _pJVal)
	{

		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		//부모클래스의 LoadJson()을 호출해서 부모클래스의 데이터를 저장
		//실패시 실패결과를 리턴
		eResult result = iComponent::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}
	
		Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		SetLightType((eLightType)mAttribute.lightType);

		//불러오기 실패 시 기본값으로 적용
		if (false == Json::SaveLoad::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute.lightType)))
		{
			mAttribute.lightType = (int)eLightType::Directional;
		}

		return eResult::Success;
	}


	void Com_Light3D::Update()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
			return;

		float3 position = tr->GetRelativePos();
		mAttribute.position = float4(position.x, position.y, position.z, 1.0f);
			
		//Transform 조작은 Update()까지만 가능.
		switch ((eLightType)mAttribute.lightType)
		{
		case ehw::eLightType::Directional:
			break;
		case ehw::eLightType::Point:
			tr->SetRelativeScale(float3(mAttribute.radius));
			break;
		case ehw::eLightType::Spot:
			break;
		case ehw::eLightType::END:
			break;
		default:
			break;
		}
	}

	void Com_Light3D::InternalUpdate()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
		{
			return;
		}

		switch ((eLightType)mAttribute.lightType)
		{
		case ehw::eLightType::Directional:
			mAttribute.direction = float4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);
			break;
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

		float3 position = tr->GetRelativePos();
		mAttribute.position = float4(position.x, position.y, position.z, 1.0f);

		RenderManager::PushLightAttribute(mAttribute);
	}


	void Com_Light3D::Render()
	{
		if (nullptr == mLightMaterial)
		{
			return;
		}

		//Transform
		iTransform* tr = static_cast<iTransform*>(GetOwner()->GetComponent(eComponentCategory::Transform));
		if (nullptr == tr)
			return;
		tr->BindData();

		//Light
		ConstBuffer* cb = RenderManager::GetConstBuffer(eCBType::numberOfLight);
		tCB_NumberOfLight data = {};
		data.numberOfLight = (uint)RenderManager::GetLights().size();
		data.indexOfLight = mIndex;

		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		mVolumeMesh->BindBuffer();
		mLightMaterial->BindData();
		mVolumeMesh->Render();
		mLightMaterial->UnBindData();
	}


	void Com_Light3D::SetLightType(eLightType type)
	{
		mAttribute.lightType = (int)type;
		if (mAttribute.lightType == (int)eLightType::Directional)
		{
			mVolumeMesh = ResourceManager::Find<Mesh>(strKey::Default::mesh::RectMesh);
			mLightMaterial = ResourceManager::Find<Material>(strKey::Default::material::LightDirMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Point)
		{
			mVolumeMesh = ResourceManager::Find<Mesh>(strKey::Default::mesh::SphereMesh);
			mLightMaterial = ResourceManager::Find<Material>(strKey::Default::material::LightPointMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Spot)
		{
			ERROR_MESSAGE_W(L"미구현");
		}
	}
	
}
