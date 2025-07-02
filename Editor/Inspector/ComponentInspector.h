#pragma once
#include <Editor/Base/EditorWindow.h>

#include <Engine/Game/Component/define_Component.h>

namespace core
{
	class GameObject;
	namespace editor
	{
		class ComponentInspector : public EditorWindow
		{
		public:
			ComponentInspector(eComponentOrder _type);
			virtual ~ComponentInspector();

			void set_target(const s_ptr<GameObject>& _target) { mTarget = _target; }
			s_ptr<GameObject> get_target() { return mTarget; }

		private:
			eComponentOrder m_ComCategory;
			s_ptr<GameObject> mTarget;
		};
	}
}

