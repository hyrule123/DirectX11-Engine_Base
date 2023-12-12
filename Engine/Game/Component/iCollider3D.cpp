#include "../../PCH_Engine.h"
#include "iCollider3D.h"



#include "../../json-cpp/json.h"


#include "Com_Transform.h"

namespace ehw
{
	iCollider3D::iCollider3D()
		: iCollider(eColliderType::None)
	{
		ASSERT(FALSE, "미구현");
	}
	
	iCollider3D::iCollider3D(const iCollider3D& _collider)
		: iCollider(_collider.m_ComCategory)
	{
		ASSERT(FALSE, "미구현");
	}

	iCollider3D::~iCollider3D()
	{
	}

	eResult iCollider3D::SaveJson(Json::Value* _pJVal)
	{
		ASSERT(FALSE, "미구현");

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = iCollider::SaveJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//Json::Value& jVal = *_pJVal;

		//Json::SaveLoad::SaveValue(_pJVal, JSONVAL(m_ComCategory));

		return eResult::Success;
	}

	eResult iCollider3D::LoadJson(const Json::Value* _pJVal)
	{
		ASSERT(FALSE, "미구현");

		//if (nullptr == _pJVal)
		//{
		//	return eResult::Fail_Nullptr;
		//}

		//eResult result = iCollider::LoadJson(_pJVal);
		//if (eResultFail(result))
		//{
		//	return result;
		//}

		//const Json::Value& jVal = (*_pJVal);

		//Json::SaveLoad::LoadValue(_pJVal, JSONVAL(m_ComCategory));
		
		return eResult::Success;
	}


}
