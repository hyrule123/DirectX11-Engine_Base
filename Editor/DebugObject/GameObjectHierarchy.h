#pragma once
#include "Editor/Base/EditorWindow.h"

namespace core
{
	class GameObject;
}

namespace core::editor
{
	class TreeNode;
	class EditorWidget_Tree;

	class GameObjectHierarchy : public EditorWindow
	{
	public:
		GameObjectHierarchy();
		virtual ~GameObjectHierarchy();

		virtual void init() override;
		virtual void update() override;

		//콜백함수 등록 용도
		void GameobjectSelectCallback(w_ptr<void> _gameobj);
		void InitializeScene();

		s_ptr<TreeNode> add_game_object_recursive(const s_ptr<GameObject>& _root_obj);

	private:
		s_ptr<EditorWidget_Tree> m_tree_widget;
	};
}
