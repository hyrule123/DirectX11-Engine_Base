#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Editor/Resource/ResourceEditor.h>

#include <Editor/Inspector/ComponentInspector.h>

namespace core
{
	class GameObject;
	class Resource;
	namespace editor
	{
		class InspectorBase : public EditorUIWindow
		{
			CLASS_INFO(InspectorBase, EditorUIWindow);
			REGISTER_FACTORY(InspectorBase);
		public:
			InspectorBase();
			virtual void init() override;

			virtual ~InspectorBase();

			virtual void update() override;
			virtual void update_UI() override;

			void set_target_gameobject(const s_ptr<GameObject>& _targetObj) { m_target_gameobject = _targetObj; }
			s_ptr<GameObject> GetTargetGameObject() { return m_target_gameobject.lock(); }

			void set_target_resource(const s_ptr<Resource>& _targetRes) { m_target_resource = _targetRes; }
			s_ptr<Resource> get_target_resource() const { return m_target_resource.lock(); }


		private:
			void indicator_button(const char* _strButtonName);

		private:
			w_ptr<GameObject> m_target_gameobject;
			w_ptr<Resource> m_target_resource;

			std::vector<s_ptr<editor::ComponentInspector>> m_component_GUIs;
			//std::vector<editor::ResourceEditor*> mGuiResources;
		};
	}
}
