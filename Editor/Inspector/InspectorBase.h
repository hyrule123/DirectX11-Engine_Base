#pragma once
#include "Editor/Base/EditorWindow.h"

#include "Editor/Resource/ResourceEditor.h"

#include "Editor/Inspector/Inspector_Component.h"

namespace ehw
{
	class GameObject;
	class iResource;
	namespace editor
	{
		class InspectorBase : public EditorWindow
		{
		public:
			InspectorBase();
			virtual ~InspectorBase();

			virtual void Init() override;
			virtual void Update() override;
			virtual void UpdateUI() override;

			void SetTargetGameObject(GameObject* _targetObj) { mTargetGameObject = _targetObj; }
			GameObject* GetTargetGameObject() { return mTargetGameObject; }

			void SetTargetResource(const std::weak_ptr<iResource>& _targetRes) { mTargetResource = _targetRes; }
			const std::weak_ptr<iResource>& GetTargetResource() const { return mTargetResource; }


		private:
			void IndicatorButton(const char* _strButtonName);

		private:
			GameObject* mTargetGameObject;
			std::weak_ptr<iResource> mTargetResource;

			std::vector<editor::Inspector_Component*> mGuiComponents;
			//std::vector<editor::ResourceEditor*> mGuiResources;
		};
	}
}
