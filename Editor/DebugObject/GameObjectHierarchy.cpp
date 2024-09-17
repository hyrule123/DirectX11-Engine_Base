#include "Editor/DebugObject/GameObjectHierarchy.h"

#include "Editor/EditorManager.h"
#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Game/Scene.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Manager/SceneManager.h>
#include <Engine/Manager/RenderManager.h>

#include <Engine/GameEngine.h>


namespace ehw::editor
{
	GameObjectHierarchy::GameObjectHierarchy()
		: EditorWindow(strKey::GameObjects)
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

	void GameObjectHierarchy::Update()
	{
		
	}



	void GameObjectHierarchy::GameobjectSelectCallback(tDataPtr _data)
	{
		GameObject* gameObj = static_cast<GameObject*>(_data.pData);

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetGameObject(gameObj);
	}

	void GameObjectHierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		Scene* scene = SceneManager::GetInst().GetActiveScene();
		if (nullptr == scene) { return; }
		std::string sceneName(scene->get_concrete_class_name());

		EditorWidget_Tree::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, tDataPtr{}, true);

		const GameObjects& gameObjects = scene->GetGameObjects();

		for (size_t i = 0; i < gameObjects.size(); ++i)
		{
			AddGameObject(root, gameObjects[i].get());
		}

		mTreeWidget->SetEnable(true);
	}

	void GameObjectHierarchy::AddGameObject(EditorWidget_Tree::tNode* parent, GameObject* gameObject)
	{
		std::string name(gameObject->GetName());
		if (name.empty())
		{
			name = "NoName";
		}

		tDataPtr data{};
		data.SetDataPtr(gameObject);

		EditorWidget_Tree::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
