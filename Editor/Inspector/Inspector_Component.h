#pragma once
#include "Editor/Base/EditorChild.h"

#include <Engine/Game/Component/define_Component.h>

namespace ehw
{
	class GameObject;
	namespace editor
	{
		class Inspector_Component : public EditorChild
		{
		public:
			Inspector_Component(eComponentCategory _type);
			virtual ~Inspector_Component();

			void SetTarget(const std::weak_ptr<GameObject>& _target) { mTarget = _target; }
			const std::weak_ptr<GameObject>& GetTarget() { return mTarget; }

		private:
			eComponentCategory m_ComCategory;
			std::weak_ptr<GameObject> mTarget;
		};
	}
}

