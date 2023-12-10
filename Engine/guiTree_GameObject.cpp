#include "PCH_Engine.h"
#include "guiTree_GameObject.h"

#include "Application.h"
#include "iScene.h"
#include "Layer.h"
#include "SceneManager.h"
#include "RenderManager.h"

#include "EditorInspector.h"
#include "EditorManager.h"
#include "EditorInspector.h"

#include "Application.h"

namespace editor
{
	guiTree_GameObject::guiTree_GameObject()
		: EditorWindow(strKey::GameObjects)
		, mTreeWidget(nullptr)
	{
		int2 winSize = ehw::Application::GetWindowSize();
		//SetSize(ImVec2((float)(winSize.x / 2), (float)(winSize.y / 2)));

		mTreeWidget = AddChild<EditorWidget_Tree>();
		

		mTreeWidget->SetEvent(this
			, std::bind(&guiTree_GameObject::GameobjectSelectCallback, this, std::placeholders::_1));

		mTreeWidget->SetDummyRoot(true);

		InitializeScene();
	}

	guiTree_GameObject::~guiTree_GameObject()
	{
	}


	void guiTree_GameObject::Update()
	{
		
	}



	void guiTree_GameObject::GameobjectSelectCallback(ehw::tDataPtr _data)
	{
		ehw::RenderManager::SetInspectorGameObject(static_cast<ehw::GameObject*>(_data.pData));
		EditorInspector* inspector = static_cast<EditorInspector*>(EditorManager::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetGameObject(ehw::RenderManager::GetInspectorGameObject());
	}

	void guiTree_GameObject::InitializeScene()
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

	void guiTree_GameObject::AddGameObject(EditorWidget_Tree::tNode* parent, ehw::GameObject* gameObject)
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
