#include "Engine/Manager/SceneManager.h"

#include "Engine/Resource/Texture.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/TimeManager.h"

#include <chrono>

namespace ehw
{
	SceneManager::SceneManager()
		: m_activeScene{nullptr}
		, m_umapSceneConstructor{}
	{
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::Init()
	{
		AtExit::AddFunc(std::bind(&SceneManager::Release, this));
	}

	void SceneManager::FixedUpdate()
	{
		if (m_activeScene)
		{
			TimeManager::GetInst().SetFixedUpdateMode(true);

			uint fixedUpdateCount = TimeManager::GetInst().GetFixedUpdateCount();

			for(uint i = 0; i < fixedUpdateCount; ++i)
			{
				m_activeScene->SceneFixedUpdate();
			}

			TimeManager::GetInst().SetFixedUpdateMode(false);
		}
	}

	void SceneManager::Update()
	{
		if (m_activeScene)
		{
			m_activeScene->SceneUpdate();
		}
	}


	void SceneManager::FinalUpdate()
	{
		if (m_activeScene)
		{
			m_activeScene->SceneFinalUpdate();
		}
	}

	void SceneManager::FrameEnd()
	{
		if (m_activeScene)
		{
			m_activeScene->SceneFrameEnd();
		}
	}

	void SceneManager::Release()
	{
		m_activeScene.reset();
		m_umapSceneConstructor.clear();
	}

	void SceneManager::Destroy()
	{
		if (m_activeScene)
		{
			m_activeScene->Destroy();
		}
	}


	void SceneManager::LoadScene(const std::string_view _strKey)
	{
		const auto& Func = m_umapSceneConstructor.find(_strKey);

		if (Func == m_umapSceneConstructor.end())
		{
			ERROR_MESSAGE("그런 이름의 씬이 없습니다.");
			return;
		}

		std::unique_ptr<Scene> NewScene = Func->second();
		if (nullptr == NewScene)
		{
			ERROR_MESSAGE("씬 생성 실패");
			return;
		}

		// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
		//ASSERT(false, "미구현");
		//std::vector<std::shared_ptr<GameObject>> dontDestroyObjs;

		//if (m_activeScene)
		//{
		//	dontDestroyObjs = m_activeScene->GetDontDestroyGameObjects();
		//}
		
		//if (m_activeScene)
		//	m_activeScene->OnExit();

		//씬 갈아끼우기
		m_activeScene.reset();
		m_activeScene = std::move(NewScene);

		//ASSERT(false, "미구현");
		//for (size_t i = 0; i < dontDestroyObjs.size(); ++i)
		//{
		//	
		//	m_activeScene->AddGameObject(dontDestroyObjs[i]->GetLayerType(), dontDestroyObjs[i]);
		//}

		//OnEnter 호출
		m_activeScene->SceneInit();
		m_activeScene->OnEnter();
		m_activeScene->SceneAwake();
	}

}
