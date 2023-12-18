#pragma once
#include "../../Editor/Base/EditorChild.h"


#include "../../Resource/iResource.h"

namespace editor
{
	class EditorResource : public EditorChild
	{
	public:
		EditorResource(const std::string_view _resTypeName);
		virtual ~EditorResource();

		void UpdateUI() override;
		
		//리소스가 지정되어 있는지 + 리소스 타입이 일치하는지를 확인 후 반환
		bool IsValid() const { ASSERT(false, "미구현"); return false; }



		void SetTarget(ehw::iResource* _target) { mTarget = _target; }
		ehw::iResource* GetTarget() const { return mTarget; }

	private:
		ehw::iResource* mTarget;
	};
}
