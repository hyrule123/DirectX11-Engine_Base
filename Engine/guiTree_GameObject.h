#pragma once
#include "guiChild.h"

#include "guiTreeWidget.h"

namespace ehw
{
	class GameObject;
}

namespace gui
{
	class guiTree_GameObject : public guiWindow
	{
	public:
		guiTree_GameObject();
		virtual ~guiTree_GameObject();

		virtual void Update() override;

		void GameobjectSelectCallback(ehw::tDataPtr _data);
		void InitializeScene();
		void AddGameObject(TreeWidget::tNode* parent, ehw::GameObject* gameObject);

	private:
		TreeWidget* mTreeWidget;
	};
}
