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

		virtual eResult Serialize_Json(JsonSerializer* _ser) override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;
		
		virtual void Init() override;
		virtual void InternalUpdate() override {};

		virtual void BindData() override;
		virtual void UnBindData() override;

		virtual bool IsPlaying() const override;

	private:
		std::shared_ptr<Com_Animator3D> mParentAnimator;
	};
}


