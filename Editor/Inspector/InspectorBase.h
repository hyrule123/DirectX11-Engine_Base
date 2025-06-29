#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Resource/ResourceEditor.h"

#include "Editor/Inspector/ComponentInspector.h"

namespace core
{
	class GameObject;
	class Resource;
	namespace editor
	{
		class InspectorBase : public EditorWindow
		{
			CLASS_INFO(InspectorBase, EditorWindow);
			REGISTER_FACTORY(InspectorBase);
		public:
			InspectorBase();
			virtual void init() override;

			virtual ~InspectorBase();

			virtual void update() override;
			virtual void update_UI() override;

			void SetTargetGameObject(const s_ptr<GameObject>& _targetObj) { mTargetGameObject = _targetObj; }
			s_ptr<GameObject> GetTargetGameObject() { return mTargetGameObject.lock(); }

			void SetTargetResource(const s_ptr<Resource>& _targetRes) { mTargetResource = _targetRes; }
			s_ptr<Resource> GetTargetResource() const { return mTargetResource.lock(); }


		private:
			void IndicatorButton(const char* _strButtonName);

		private:
			w_ptr<GameObject> mTargetGameObject;
			w_ptr<Resource> mTargetResource;

			std::vector<s_ptr<editor::ComponentInspector>> mGuiComponents;
			//std::vector<editor::ResourceEditor*> mGuiResources;
		};
	}
}
