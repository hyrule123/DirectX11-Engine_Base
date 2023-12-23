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
		auto parent = GetOwner()->GetParent();

		ASSERT(nullptr != parent, "DummyTransform은 Parent가 반드시 존재해야 합니다.");

		mParentTransform = parent->GetComponent<Com_Transform>();
	}

	void Com_DummyTransform::BindData()
	{
		if (mParentTransform)
		{
			mParentTransform->BindData();
		}
	}
}

