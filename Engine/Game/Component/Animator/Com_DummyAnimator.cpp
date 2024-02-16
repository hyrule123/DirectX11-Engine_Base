#include "Com_DummyAnimator.h"



#include "../../GameObject.h"

#include "Com_Animator3D.h"

namespace ehw
{

	Com_DummyAnimator::Com_DummyAnimator()
		: iAnimator(eDimensionType::_3D)
		, mParentAnimator()
	{
	}

	Com_DummyAnimator::~Com_DummyAnimator()
	{
	}

	eResult Com_DummyAnimator::Serialize_Json(JsonSerializer* _ser)
	{
		return eResult();
	}

	eResult Com_DummyAnimator::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_DummyAnimator::Init()
	{
		const auto& parent = GetOwner()->GetParent();
		if (parent)
		{
			mParentAnimator = parent->GetComponent<Com_Animator3D>();
		}
	}

	void Com_DummyAnimator::BindData()
	{
		if (mParentAnimator)
			mParentAnimator->BindData();
	}

	void Com_DummyAnimator::UnBindData()
	{
		if (mParentAnimator)
			mParentAnimator->UnBindData();
	}
	bool Com_DummyAnimator::IsPlaying() const
	{
		bool bPlaying = false;
		if (mParentAnimator)
			bPlaying = mParentAnimator->IsPlaying();
		return bPlaying;
	}
}

