#include "PCH_Engine.h"

#include "Com_Light3D.h"
#include "Com_Transform.h"
#include "GameObject.h"
#include "RenderMgr.h"
#include "ResourceMgr.h"
#include "Mesh.h"
#include "Material.h"

#include "ConstBuffer.h"

#include "json-cpp/json.h"

namespace mh
{
	Com_Light3D::Com_Light3D()
		: ILight(eDimensionType::_3D)
		, mVolumeMesh()
		, mLightMaterial()
		, mIndex()
	{
		RenderMgr::AddLight(this);

	}

	Com_Light3D::Com_Light3D(const Com_Light3D& _other)
		: ILight(_other)
		, mIndex(_other.mIndex)
		, mAttribute(_other.mAttribute)
		, mVolumeMesh(_other.mVolumeMesh)
		, mLightMaterial(_other.mLightMaterial)
	{
		RenderMgr::AddLight(this);
	}

	Com_Light3D::~Com_Light3D()
	{
		RenderMgr::RemoveLight(this);
	}

	eResult Com_Light3D::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = IComponent::SaveJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}

		Json::Value& jVal = *_pJVal;

		Json::MH::SaveValue(_pJVal, JSON_KEY_PAIR(mAttribute));

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
		eResult result = IComponent::LoadJson(_pJVal);
		if (eResultFail(result))
		{
			return result;
		}
	
		Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute));

		SetLightType((eLightType)mAttribute.lightType);

		//불러오기 실패 시 기본값으로 적용
		if (false == Json::MH::LoadValue(_pJVal, JSON_KEY_PAIR(mAttribute.lightType)))
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
		case mh::define::eLightType::Directional:
			break;
		case mh::define::eLightType::Point:
			tr->SetRelativeScale(float3(mAttribute.radius));
			break;
		case mh::define::eLightType::Spot:
			break;
		case mh::define::eLightType::END:
			break;
		default:
			break;
		}
	}

	void Com_Light3D::FixedUpdate()
	{
		Com_Transform* tr = GetOwner()->GetComponent<Com_Transform>();
		if (nullptr == tr)
		{
			return;
		}

		switch ((eLightType)mAttribute.lightType)
		{
		case mh::define::eLightType::Directional:
			mAttribute.direction = float4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);
			break;
		case mh::define::eLightType::Point:
			//Update에서 수행했음.
			break;
		case mh::define::eLightType::Spot:
			break;
		case mh::define::eLightType::END:
			break;
		default:
			break;
		}

		float3 position = tr->GetRelativePos();
		mAttribute.position = float4(position.x, position.y, position.z, 1.0f);

		RenderMgr::PushLightAttribute(mAttribute);
	}


	void Com_Light3D::Render()
	{
		if (nullptr == mLightMaterial)
		{
			return;
		}

		//Transform
		ITransform* tr = static_cast<ITransform*>(GetOwner()->GetComponent(eComponentType::Transform));
		if (nullptr == tr)
			return;
		tr->BindData();

		//Light
		ConstBuffer* cb = RenderMgr::GetConstBuffer(eCBType::numberOfLight);
		tCB_NumberOfLight data = {};
		data.numberOfLight = (uint)RenderMgr::GetLights().size();
		data.indexOfLight = mIndex;

		cb->SetData(&data);
		cb->BindData(eShaderStageFlag::VS | eShaderStageFlag::PS);

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
			mVolumeMesh = ResourceMgr::Find<Mesh>(strKey::Default::mesh::RectMesh);
			mLightMaterial = ResourceMgr::Find<Material>(strKey::Default::material::LightDirMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Point)
		{
			mVolumeMesh = ResourceMgr::Find<Mesh>(strKey::Default::mesh::SphereMesh);
			mLightMaterial = ResourceMgr::Find<Material>(strKey::Default::material::LightPointMaterial);
		}
		else if (mAttribute.lightType == (int)eLightType::Spot)
		{
			ERROR_MESSAGE_W(L"미구현");
		}
	}
	
}
