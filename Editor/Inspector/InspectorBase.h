#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Resource/ResourceEditor.h"

#include "Editor/Inspector/Inspector_Component.h"

namespace core
{
	class GameObject;
	class Resource;
	namespace editor
	{
		class InspectorBase : public EditorWindow
		{
		public:
			InspectorBase();
			virtual ~InspectorBase();

			virtual void init() override;
			virtual void update() override;
			virtual void update_UI() override;

			void SetTargetGameObject(GameObject* _targetObj) { mTargetGameObject = _targetObj; }
			GameObject* GetTargetGameObject() { return mTargetGameObject; }

			void SetTargetResource(const std::weak_ptr<Resource>& _targetRes) { mTargetResource = _targetRes; }
			const std::weak_ptr<Resource>& GetTargetResource() const { return mTargetResource; }


		private:
			void IndicatorButton(const char* _strButtonName);

		private:
			GameObject* mTargetGameObject;
			std::weak_ptr<Resource> mTargetResource;

			std::vector<editor::Inspector_Component*> mGuiComponents;
			//std::vector<editor::ResourceEditor*> mGuiResources;
		};
	}
}
