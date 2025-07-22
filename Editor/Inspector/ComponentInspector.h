#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Engine/Game/Component/define_Component.h>

namespace core
{
	class GameObject;
	namespace editor
	{
		class ComponentInspector : public EditorUIWindow
		{
			CLASS_INFO(ComponentInspector, EditorUIWindow);
		public:
			ComponentInspector(eComponentOrder _type);
			virtual void init() override;

			virtual ~ComponentInspector();

			void set_target(const s_ptr<GameObject>& _target) { m_target_gameobject = _target; }
			s_ptr<GameObject> get_target() { return m_target_gameobject; }

		private:
			eComponentOrder m_component_order;
			s_ptr<GameObject> m_target_gameobject;
		};
	}
}

