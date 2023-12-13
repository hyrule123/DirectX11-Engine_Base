#include "Com_DummyTransform.h"



#include "../../GameObject.h"

#include "Com_Transform.h"


namespace ehw
{
	Com_DummyTransform::Com_DummyTransform()
		: mParentTransform()
	{
	}

	Com_DummyTransform::~Com_DummyTransform()
	{
	}

	void Com_DummyTransform::Init()
	{
		GameObject* parent = GetOwner()->GetParent();
		if (parent)
		{
			mParentTransform = parent->GetComponent<Com_Transform>();
		}
	}

	void Com_DummyTransform::BindData()
	{
		if (mParentTransform)
			mParentTransform->BindData();
	}
}

