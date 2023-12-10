#pragma once
#include "EditorChild.h"
#include "GameObject.h"


namespace editor
{
	class guiComponent : public EditorChild
	{
	public:
		guiComponent(ehw::eComponentCategory _type);
		virtual ~guiComponent();
		
		void SetTarget(ehw::GameObject* _target) { mTarget = _target; }
		ehw::GameObject* GetTarget() { return mTarget; }

	private:
		ehw::eComponentCategory m_ComCategory;
		ehw::GameObject* mTarget;
	};
}
