#include "iCollider3D.h"



#include "../Transform/Com_transform.h"

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

	eResult iCollider3D::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult iCollider3D::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}
}
