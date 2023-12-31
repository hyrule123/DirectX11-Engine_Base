
#include "EditorGameObject.h"

#include "../../Application.h"
#include "../../Game/iScene.h"
#include "../../Game/Layer.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/RenderManager.h"

#include "../../Manager/EditorManager.h"

#include "../../Editor/Inspector/EditorInspector.h"

namespace editor
{
	EditorGameObject::EditorGameObject()
		: EditorWindow(strKey::GameObjects)
		, mTreeWidget(nullptr)
	{

	}

	EditorGameObject::~EditorGameObject()
	{
	}


	void EditorGameObject::Init()
	{
		mTreeWidget = AddChild<EditorWidget_Tree>();


		mTreeWidget->SetEvent(this
			, std::bind(&EditorGameObject::GameobjectSelectCallback, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	void EditorGameObject::Update()
	{
		
	}



	void EditorGameObject::GameobjectSelectCallback(ehw::tDataPtr _data)
	{
		ASSERT(false, "미구현");

		/*

		auto gameObj = static_cast<ehw::GameObject*>(_data.pData)->shared_from_this_T<ehw::GameObject>();

		
		ehw::RenderManager::SetInspectorGameObject(gameObj);

		std::shared_ptr<EditorInspector> inspector = std::static_pointer_cast<EditorInspector>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetGameObject(ehw::RenderManager::GetInspectorGameObject());

		*/
	}

	void EditorGameObject::InitializeScene()
	{
		mTreeWidget->Clear();

		ehw::iScene* scene = ehw::SceneManager::GetActiveScene();
		std::string sceneName = scene->GetStrKey();

		EditorWidget_Tree::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, ehw::tDataPtr{}, true);

		for (size_t i = 0; i < (UINT)ehw::eLayerType::END; i++)
		{
			ehw::Layer& layer = scene->GetLayer((ehw::eLayerType)i);
			const std::vector<std::shared_ptr<ehw::GameObject>>& gameObjs
				= layer.GetGameObjects();

			for (const std::shared_ptr<ehw::GameObject>& obj : gameObjs)
			{
				AddGameObject(root, obj.get());
			}
		}

		mTreeWidget->SetEnable(true);
	}

	void EditorGameObject::AddGameObject(EditorWidget_Tree::tNode* parent, ehw::GameObject* gameObject)
	{
		std::string name(gameObject->GetName());
		if (name.empty())
		{
			name = "NoNameObj";
		}

		ehw::tDataPtr data{};
		data.SetDataPtr(gameObject);

		EditorWidget_Tree::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
