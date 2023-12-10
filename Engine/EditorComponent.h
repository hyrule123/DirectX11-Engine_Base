#pragma once
#include "EditorChild.h"
#include "GameObject.h"


namespace editor
{
	class EditorComponent : public EditorChild
	{
	public:
		EditorComponent(ehw::eComponentCategory _type);
		virtual ~EditorComponent();
		
		void SetTarget(ehw::GameObject* _target) { mTarget = _target; }
		ehw::GameObject* GetTarget() { return mTarget; }

	private:
		ehw::eComponentCategory m_ComCategory;
		ehw::GameObject* mTarget;
	};
}
