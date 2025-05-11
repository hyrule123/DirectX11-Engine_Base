#pragma once
#include "Editor/Base/EditorChild.h"

#include <Engine/Game/Component/define_Component.h>

namespace core
{
	class GameObject;
	namespace editor
	{
		class Inspector_Component : public EditorChild
		{
		public:
			Inspector_Component(eComponentOrder _type);
			virtual ~Inspector_Component();

			void SetTarget(GameObject* _target) { mTarget = _target; }
			GameObject* GetTarget() { return mTarget; }

		private:
			eComponentOrder m_ComCategory;
			GameObject* mTarget;
		};
	}
}

