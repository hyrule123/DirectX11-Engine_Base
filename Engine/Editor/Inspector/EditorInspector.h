#pragma once
#include "../Base/EditorWindow.h"



#include "../../Game/GameObject.h"

#include "../Resource/EditorResource.h"

#include "EditorComponent.h"

namespace editor
{
	class EditorInspector : public EditorWindow
	{
	public:
		EditorInspector();
		virtual ~EditorInspector();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

		void SetTargetGameObject(ehw::GameObject* _targetObj) { mTargetGameObject = _targetObj; }
		ehw::GameObject* GetTargetGameObject() const { return mTargetGameObject; }

		void SetTargetResource(ehw::iResource* _targetRes) { mTargetResource = _targetRes; }
		ehw::iResource* GetTargetResource() const { return mTargetResource; }


	private:
		void IndicatorButton(const char* _strButtonName);

	private:
		ehw::GameObject* mTargetGameObject;
		ehw::iResource* mTargetResource;

		std::vector<editor::EditorComponent*> mGuiComponents;
		std::vector<editor::EditorResource*> mGuiResources;
	};
}
