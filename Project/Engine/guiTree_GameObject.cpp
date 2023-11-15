#include "PCH_Engine.h"
#include "guiTree_GameObject.h"

#include "Application.h"
#include "IScene.h"
#include "Layer.h"
#include "SceneMgr.h"
#include "RenderMgr.h"

#include "guiInspector.h"
#include "guiMgr.h"
#include "guiInspector.h"

#include "Application.h"

namespace gui
{
	guiTree_GameObject::guiTree_GameObject()
		: guiWindow(strKey::GameObjects)
		, mTreeWidget(nullptr)
	{
		int2 winSize = ehw::Application::GetWindowSize();
		//SetSize(ImVec2((float)(winSize.x / 2), (float)(winSize.y / 2)));

		mTreeWidget = AddChild<TreeWidget>();
		

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



	void guiTree_GameObject::GameobjectSelectCallback(ehw::define::tDataPtr _data)
	{
		ehw::RenderMgr::SetInspectorGameObject(static_cast<ehw::GameObject*>(_data.pData));
		guiInspector* inspector = static_cast<guiInspector*>(guiMgr::FindGuiWindow(strKey::Inspector));
		inspector->SetTargetGameObject(ehw::RenderMgr::GetInspectorGameObject());
	}

	void guiTree_GameObject::InitializeScene()
	{
		mTreeWidget->Clear();

		ehw::IScene* scene = ehw::SceneMgr::GetActiveScene();
		std::string sceneName = scene->GetKey();

		TreeWidget::tNode* root = mTreeWidget->AddNode(nullptr, sceneName, ehw::define::tDataPtr{}, true);

		for (size_t i = 0; i < (UINT)ehw::define::eLayerType::END; i++)
		{
			ehw::Layer& layer = scene->GetLayer((ehw::define::eLayerType)i);
			const std::vector<ehw::GameObject*>& gameObjs
				= layer.GetGameObjects();

			for (ehw::GameObject* obj : gameObjs)
			{
				AddGameObject(root, obj);
			}
		}

		mTreeWidget->SetEnable(true);
	}

	void guiTree_GameObject::AddGameObject(TreeWidget::tNode* parent, ehw::GameObject* gameObject)
	{
		std::string name(gameObject->GetName());
		if (name.empty())
		{
			name = "NoNameObj";
		}

		ehw::define::tDataPtr data{};
		data.SetDataPtr(gameObject);

		TreeWidget::tNode* node = mTreeWidget->AddNode(parent, name, data);
	}

}
