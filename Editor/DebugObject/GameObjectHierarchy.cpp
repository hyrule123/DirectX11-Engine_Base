#include "Editor/DebugObject/GameObjectHierarchy.h"

#include "Editor/EditorManager.h"
#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Game/iScene.h>
#include <Engine/Game/Layer.h>

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


	void GameObjectHierarchy::Init()
	{
		mTreeWidget = AddChild<EditorWidget_Tree>();


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
		ASSERT(false, "미구현");

		/*

		auto gameObj = static_cast<GameObject*>(_data.pData)->shared_from_this_T<GameObject>();

		
		RenderManager::SetInspectorGameObject(gameObj);

		std::shared_ptr<InspectorBase> inspector = std::static_pointer_cast<InspectorBase>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetGameObject(RenderManager::GetInspectorGameObject());

		*/
	}

	void GameObjectHierarchy::InitializeScene()
	{
		mTreeWidget->Clear();

		iScene* scene = SceneManager::GetActiveScene();
		std::string sceneName = scene->GetStrKey();

		EditorWidget_Tree::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, tDataPtr{}, true);

		for (size_t i = 0; i < (UINT)eLayer::END; i++)
		{
			Layer& layer = scene->GetLayer((eLayer)i);
			const std::vector<std::shared_ptr<GameObject>>& gameObjs
				= layer.GetGameObjects();

			for (const std::shared_ptr<GameObject>& obj : gameObjs)
			{
				AddGameObject(root, obj.get());
			}
		}

		mTreeWidget->SetEnable(true);
	}

	void GameObjectHierarchy::AddGameObject(EditorWidget_Tree::tNode* parent, GameObject* gameObject)
	{
		std::string name(gameObject->GetName());
		if (name.empty())
		{
			name = "NoNameObj";
		}

		tDataPtr data{};
		data.SetDataPtr(gameObject);

		EditorWidget_Tree::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
