#pragma once
#include "Editor/Base/EditorChild.h"
#include "Editor/Widget/Widget_Tree.h"

namespace core
{
	class GameObject;
}

namespace core::editor
{
	class GameObjectHierarchy : public EditorWindow
	{
	public:
		GameObjectHierarchy();
		virtual ~GameObjectHierarchy();

		virtual void init() override;
		virtual void update() override;

		void GameobjectSelectCallback(tDataPtr _data);
		void InitializeScene();
		void AddGameObject(EditorWidget_Tree::tNode* parent, GameObject* get_owner);

	private:
		std::shared_ptr<EditorWidget_Tree> mTreeWidget;
	};
}
