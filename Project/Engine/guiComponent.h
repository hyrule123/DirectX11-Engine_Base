#pragma once
#include "guiChild.h"
#include "GameObject.h"


namespace gui
{
	class guiComponent : public guiChild
	{
	public:
		guiComponent(ehw::define::eComponentType _type);
		virtual ~guiComponent();
		
		void SetTarget(ehw::GameObject* _target) { mTarget = _target; }
		ehw::GameObject* GetTarget() { return mTarget; }

	private:
		ehw::define::eComponentType mType;
		ehw::GameObject* mTarget;
	};
}
