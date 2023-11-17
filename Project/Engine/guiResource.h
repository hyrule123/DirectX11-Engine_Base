#pragma once
#include "guiChild.h"
#include "IResource.h"

namespace gui
{
	class guiResource : public guiChild
	{
	public:
		guiResource(ehw::define::eResourceType _resType);
		virtual ~guiResource();

		void UpdateUI() override;
		
		//리소스가 지정되어 있는지 + 리소스 타입이 일치하는지를 확인 후 반환
		bool IsValid() const { return (mTarget && mTarget->GetResType() == mResType); }

		ehw::eResourceType GetResType() { return mResType; }

		void SetTarget(ehw::IResource* _target) { mTarget = _target; }
		ehw::IResource* GetTarget() const { return mTarget; }

	private:
		ehw::eResourceType mResType;
		ehw::IResource* mTarget;
	};
}
