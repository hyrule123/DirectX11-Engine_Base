#include "Editor/DebugObject/GameObjectHierarchy.h"

#include "Editor/EditorManager.h"
#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Game/Scene.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Manager/SceneManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/GameEngine.h>


namespace core::editor
{
	GameObjectHierarchy::GameObjectHierarchy()
		: EditorWindow(name::GameObjects)
		, mTreeWidget(nullptr)
	{

	}

	GameObjectHierarchy::~GameObjectHierarchy()
	{
	}


	void GameObjectHierarchy::init()
	{
		mTreeWidget = add_child<EditorWidget_Tree>();


		mTreeWidget->SetEvent(this
			, std::bind(&GameObjectHierarchy::GameobjectSelectCallback, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	void GameObjectHierarchy::update()
	{
		
	}



	void GameObjectHierarchy::GameobjectSelectCallback(tDataPtr _data)
	{
		GameObject* gameObj = static_cast<GameObject*>(_data.pData);

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow(name::Inspector));
		inspector->SetTargetGameObject(gameObj);
	}

	void GameObjectHierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		s_ptr<Scene> scene = SceneManager::get_inst().GetActiveScene();
		if (nullptr == scene) { return; }
		std::string sceneName(scene->get_concrete_class_name());

		EditorWidget_Tree::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, tDataPtr{}, true);

		const std::vector<s_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

		for (size_t i = 0; i < gameObjects.size(); ++i)
		{
			AddGameObject(root, gameObjects[i].get());
		}

		mTreeWidget->SetEnable(true);
	}

	void GameObjectHierarchy::AddGameObject(EditorWidget_Tree::tNode* parent, GameObject* get_owner)
	{
		std::string name(get_owner->GetName());
		if (name.empty())
		{
			name = "NoName";
		}

		tDataPtr data{};
		data.SetDataPtr(get_owner);

		EditorWidget_Tree::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
