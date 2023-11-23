#include "PCH_Engine.h"
#include "ICollider3D.h"
#include "Com_Transform.h"

#include "json-cpp/json.h"

namespace ehw
{
	using namespace ehw::define;

	ICollider3D::ICollider3D()
		: ICollider(define::eColliderType::None)
	{
		ASSERT(FALSE, "미구현");
	}
	
	ICollider3D::ICollider3D(const ICollider3D& _collider)
		: ICollider(_collider.mType)
	{
		ASSERT(FALSE, "미구현");
	}

	ICollider3D::~ICollider3D()
	{
	}

	eResult ICollider3D::SaveJson(Json::Value* _pJVal)
	{
		ASSERT(FALSE, "미구현");

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = ICollider::SaveJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//Json::Value& jVal = *_pJVal;

		//Json::MH::SaveValue(_pJVal, JSONVAL(mType));

		return eResult::Success;
	}

	eResult ICollider3D::LoadJson(const Json::Value* _pJVal)
	{
		ASSERT(FALSE, "미구현");

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = ICollider::LoadJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//const Json::Value& jVal = (*_pJVal);

		//Json::MH::LoadValue(_pJVal, JSONVAL(mType));
		
		return eResult::Success;
	}


}
