#pragma once
#include "guiChild.h"
#include "GameObject.h"
#include "guiComponent.h"
#include "guiResource.h"

namespace gui
{
	class guiInspector : public guiWindow
	{
	public:
		guiInspector();
		virtual ~guiInspector();

		virtual void Init() override;
		virtual void Update() override;
		virtual void UpdateUI() override;

		void SetTargetGameObject(ehw::GameObject* _targetObj) { mTargetGameObject = _targetObj; }
		ehw::GameObject* GetTargetGameObject() const { return mTargetGameObject; }

		void SetTargetResource(ehw::IResource* _targetRes) { mTargetResource = _targetRes; }
		ehw::IResource* GetTargetResource() const { return mTargetResource; }


	private:
		void IndicatorButton(const char* _strButtonName);

	private:
		ehw::GameObject* mTargetGameObject;
		ehw::IResource* mTargetResource;

		std::vector<gui::guiComponent*> mGuiComponents;
		std::vector<gui::guiResource*> mGuiResources;
	};
}