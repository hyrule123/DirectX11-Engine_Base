#pragma once
#include "iAnimator.h"

namespace ehw
{
	class Com_Animator3D;
	class Com_DummyAnimator :
		public iAnimator
	{
	public:
		Com_DummyAnimator();
		virtual ~Com_DummyAnimator();
		
		virtual void Init() override;
		virtual void InternalUpdate() override {};

		virtual void BindData() override;
		virtual void UnBindData() override;

		virtual bool IsPlaying() const override;

	private:
		Com_Animator3D* mParentAnimator;
	};
}


