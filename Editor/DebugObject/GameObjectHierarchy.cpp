#include <Editor/DebugObject/GameObjectHierarchy.h>

#include <Editor/EditorManager.h>
#include <Editor/Inspector/InspectorBase.h>
#include <Editor/Widget/Widget_Tree.h>

#include <Engine/Game/Scene.h>

#include <Engine/Game/GameObject.h>
#include <Engine/Game/Component/Transform.h>

#include <Engine/Manager/SceneManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/GameEngine.h>


namespace core::editor
{
	GameObjectHierarchy::GameObjectHierarchy()
		: EditorWindow(name::GameObjects)
		, m_tree_widget(nullptr)
	{

	}

	GameObjectHierarchy::~GameObjectHierarchy()
	{
	}


	void GameObjectHierarchy::init()
	{
		m_tree_widget = new_entity<EditorWidget_Tree>();

		m_tree_widget->set_selected_callaback_func(
			std::bind(&GameObjectHierarchy::GameobjectSelectCallback, this, std::placeholders::_1));

		InitializeScene();
	}

	void GameObjectHierarchy::update()
	{
	}

	void GameObjectHierarchy::GameobjectSelectCallback(w_ptr<void> _gameobj)
	{
		s_ptr<GameObject> obj = std::reinterpret_pointer_cast<GameObject>(_gameobj.lock());

		//최소한의 잘못된 캐스팅 방어 수단
		if (obj->get_static_type_name() != GameObject::s_static_type_name)
		{
			DEBUG_MESSAGE("reinterpret_cast 실패");
			return;
		}

		s_ptr<InspectorBase> inspector = 
			std::static_pointer_cast<InspectorBase>(EditorManager::get_inst().find_editor_window(name::Inspector));
		inspector->SetTargetGameObject(obj);
	}

	void GameObjectHierarchy::InitializeScene()
	{
		m_tree_widget->clear();

		s_ptr<Scene> scene = SceneManager::get_inst().get_active_scene();
		if (nullptr == scene) { return; }
		std::string sceneName(scene->get_static_type_name());

		TreeNode& root = m_tree_widget->get_root();
		root.set_unique_name(sceneName);	

		const std::vector<s_ptr<GameObject>>& gameObjects = scene->get_game_objects();

		for (const s_ptr<GameObject>& obj : gameObjects)
		{
			if (obj->get_transform()->is_root())
			{
				s_ptr<TreeNode> root = add_game_object_recursive(obj);
				m_tree_widget->get_root().add_child(root);
			}
		}

		m_tree_widget->set_enable(true);
	}

	s_ptr<TreeNode> GameObjectHierarchy::add_game_object_recursive(const s_ptr<GameObject>& _obj)
	{
		if (nullptr == _obj) { return nullptr; }
		
		s_ptr<TreeNode> ret = new_entity<TreeNode>();
		const std::string& obj_name = _obj->get_name();
		if (obj_name.empty())
		{
			ret->set_unique_name("Nameless Object");
		}
		else
		{
			ret->set_unique_name(obj_name);
		}
		ret->set_data_ptr(_obj);

		const std::vector<w_ptr<Transform>>& childs = _obj->get_transform()->get_childs();
		for (const w_ptr<Transform>& child : childs)
		{
			if (child.lock())
			{
				s_ptr<TreeNode> child_node = add_game_object_recursive(child.lock()->get_owner());
				if (child_node)
				{
					ret->add_child(child_node);
				}
			}
		}

		return ret;
	}

}
