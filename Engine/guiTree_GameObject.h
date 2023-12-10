#pragma once
#include "EditorChild.h"

#include "EditorWidget_Tree.h"

namespace ehw
{
	class GameObject;
}

namespace editor
{
	class guiTree_GameObject : public EditorWindow
	{
	public:
		guiTree_GameObject();
		virtual ~guiTree_GameObject();

		virtual void Update() override;

		void GameobjectSelectCallback(ehw::tDataPtr _data);
		void InitializeScene();
		void AddGameObject(EditorWidget_Tree::tNode* parent, ehw::GameObject* gameObject);

	private:
		EditorWidget_Tree* mTreeWidget;
	};
}
