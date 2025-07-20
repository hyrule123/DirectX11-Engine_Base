#pragma once
#include <Editor/Base/EditorUIWindow.h>

namespace core
{
	class GameObject;
}

namespace core::editor
{
	class TreeNode;
	class HierarchyTreeWidget;

	class GameObjectHierarchy : public EditorUIWindow
	{
	public:
		GameObjectHierarchy();
		virtual ~GameObjectHierarchy();

		virtual void init() override;
		virtual void update() override;

		//콜백함수 등록 용도
		void callback_func_when_selected(w_ptr<void> _gameobj);
		void initialize_scene();

		s_ptr<TreeNode> add_game_object_recursive(const s_ptr<GameObject>& _root_obj);

	private:
		s_ptr<HierarchyTreeWidget> m_tree_widget;
	};
}
