#pragma once
#include "Base/EditorWindow.h"

#include "Game/GameObject.h"

#include "Resource/EditorResource.h"

#include "Inspector/EditorComponent.h"

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

		void SetTargetGameObject(const std::shared_ptr<ehw::GameObject>& _targetObj) { mTargetGameObject = _targetObj; }
		const std::weak_ptr<ehw::GameObject>& GetTargetGameObject() const { return mTargetGameObject; }

		void SetTargetResource(const std::weak_ptr<ehw::iResource>& _targetRes) { mTargetResource = _targetRes; }
		const std::weak_ptr<ehw::iResource>& GetTargetResource() const { return mTargetResource; }


	private:
		void IndicatorButton(const char* _strButtonName);

	private:
		std::weak_ptr<ehw::GameObject> mTargetGameObject;
		std::weak_ptr<ehw::iResource> mTargetResource;

		std::vector<editor::EditorComponent*> mGuiComponents;
		//std::vector<editor::EditorResource*> mGuiResources;
	};
}
