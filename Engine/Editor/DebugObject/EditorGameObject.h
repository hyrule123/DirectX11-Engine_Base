#pragma once
#include "../../Editor/Base/EditorChild.h"

#include "../../Editor/Widget/EditorWidget_Tree.h"

namespace ehw
{
	class GameObject;
}

namespace editor
{
	class EditorGameObject : public EditorWindow
	{
	public:
		EditorGameObject();
		virtual ~EditorGameObject();

		virtual void Update() override;

		void GameobjectSelectCallback(ehw::tDataPtr _data);
		void InitializeScene();
		void AddGameObject(EditorWidget_Tree::tNode* parent, ehw::GameObject* gameObject);

	private:
		EditorWidget_Tree* mTreeWidget;
	};
}