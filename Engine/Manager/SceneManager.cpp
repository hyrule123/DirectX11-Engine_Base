#include "Engine/Manager/SceneManager.h"


#include "Engine/Resource/Texture.h"

#include "Engine/Util/AtExit.h"


#include "RenderManager.h"
#include "ResourceManager.h"

namespace ehw
{
	std::unique_ptr<iScene> SceneManager::mActiveScene = nullptr;

	std::unordered_map<std::string_view, std::function<iScene* ()>> SceneManager::mUmapSceneConstructor{};

	void SceneManager::Init()
	{
		AtExit::AddFunc(Release);
	}

	void SceneManager::Update()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneUpdate();
		}
			
	}

	void SceneManager::CollisionUpdate()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneCollisionUpdate();
		}
	}

	void SceneManager::FinalUpdate()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneFinalUpdate();
		}
			
	}

	void SceneManager::Render()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneRender();
		}
			
	}
	
	void SceneManager::RemoveDestroyed()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneRemoveDestroyed();
		}	
	}

	void SceneManager::FrameEnd()
	{
		if (mActiveScene)
		{
			mActiveScene->SceneFrameEnd();
		}
	}

	void SceneManager::Release()
	{
		mActiveScene.reset();
		mUmapSceneConstructor.clear();
	}


	void SceneManager::LoadScene(const std::string_view _strKey)
	{
		const auto& Func = mUmapSceneConstructor.find(_strKey);

		if (Func == mUmapSceneConstructor.end())
		{
			ERROR_MESSAGE("그런 이름의 씬이 없습니다.");
			return;
		}

		std::unique_ptr<iScene> NewScene = std::unique_ptr<iScene>(Func->second());
		if (nullptr == NewScene)
		{
			ERROR_MESSAGE("씬 생성 실패");
			return;
		}

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		//ASSERT(false, "미구현");
		//std::vector<std::shared_ptr<GameObject>> dontDestroyObjs;

		//if (mActiveScene)
		//{
		//	dontDestroyObjs = mActiveScene->GetDontDestroyGameObjects();
		//}
		
		if (mActiveScene)
			mActiveScene->OnExit();

		//씬 갈아끼우기
		mActiveScene.reset();
		mActiveScene = std::move(NewScene);

		//ASSERT(false, "미구현");
		//for (size_t i = 0; i < dontDestroyObjs.size(); ++i)
		//{
		//	
		//	mActiveScene->AddGameObject(dontDestroyObjs[i]->GetLayerType(), dontDestroyObjs[i]);
		//}

		//OnEnter 호출
		mActiveScene->OnEnter();

		mActiveScene->SceneAwake();
	}

}
