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

	void Com_DummyAnimator::Init()
	{
		GameObject* parent = GetOwner()->GetParent();
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

