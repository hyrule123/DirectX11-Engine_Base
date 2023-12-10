#pragma once
#include "EditorChild.h"
#include "iResource.h"

namespace editor
{
	class guiResource : public EditorChild
	{
	public:
		guiResource(ehw::eResourceType _resType);
		virtual ~guiResource();

		void UpdateUI() override;
		
		//리소스가 지정되어 있는지 + 리소스 타입이 일치하는지를 확인 후 반환
		bool IsValid() const { return (mTarget && mTarget->GetResType() == mResType); }

		ehw::eResourceType GetResType() { return mResType; }

		void SetTarget(ehw::iResource* _target) { mTarget = _target; }
		ehw::iResource* GetTarget() const { return mTarget; }

	private:
		ehw::eResourceType mResType;
		ehw::iResource* mTarget;
	};
}
