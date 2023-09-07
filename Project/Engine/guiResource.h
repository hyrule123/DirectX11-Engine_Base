#pragma once
#include "guiChild.h"
#include "IRes.h"

namespace gui
{
	class guiResource : public guiChild
	{
	public:
		guiResource(mh::define::eResourceType _resType);
		virtual ~guiResource();

		void UpdateUI() override;
		
		//리소스가 지정되어 있는지 + 리소스 타입이 일치하는지를 확인 후 반환
		bool IsValid() const { return (mTarget && mTarget->GetResType() == mResType); }

		mh::eResourceType GetResType() { return mResType; }

		void SetTarget(mh::IRes* _target) { mTarget = _target; }
		mh::IRes* GetTarget() const { return mTarget; }

	private:
		mh::eResourceType mResType;
		mh::IRes* mTarget;
	};
}
