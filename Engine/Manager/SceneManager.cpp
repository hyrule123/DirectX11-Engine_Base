#include "Engine/Manager/SceneManager.h"

#include "Engine/Resource/Texture.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/TimeManager.h"

#include <chrono>

namespace core
{
	SceneManager::SceneManager()
		: m_activeScene{nullptr}
	{
		AtExit::add_func(SceneManager::destroy_inst);
	}

	SceneManager::~SceneManager()
	{
		SAFE_DELETE(m_activeScene);
	}

	void SceneManager::fixed_update()
	{
		if (m_activeScene)
		{
			TimeManager::get_inst().SetFixedUpdateMode(true);

			uint fixedUpdateCount = TimeManager::get_inst().GetFixedUpdateCount();

			for(uint i = 0; i < fixedUpdateCount; ++i)
			{
				m_activeScene->fixed_update();
			}

			TimeManager::get_inst().SetFixedUpdateMode(false);
		}
	}

	void SceneManager::update()
	{
		if (m_activeScene)
		{
			m_activeScene->update();
		}
	}


	void SceneManager::final_update()
	{
		if (m_activeScene)
		{
			m_activeScene->final_update();
		}
	}

	void SceneManager::frame_end()
	{
		if (m_activeScene)
		{
			m_activeScene->frame_end();
		}
	}

	void SceneManager::Destroy()
	{
		if (m_activeScene)
		{
			m_activeScene->destroy();
		}
	}

	Scene* SceneManager::LoadScene(std::unique_ptr<Scene> _scene)
	{
		ASSERT(nullptr != _scene, "해당 이름의 씬이 없습니다.");

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
		SAFE_DELETE(m_activeScene);
		m_activeScene = _scene.release();

		//ASSERT(false, "미구현");
		//for (size_t i = 0; i < dontDestroyObjs.size(); ++i)
		//{
		//	
		//	m_activeScene->AddGameObject(dontDestroyObjs[i]->GetLayerType(), dontDestroyObjs[i]);
		//}

		//OnEnter 호출
		m_activeScene->init();
		m_activeScene->on_enter();
		m_activeScene->awake();

		return m_activeScene;
	}

	Scene* SceneManager::LoadScene(const std::string_view _resource_name)
	{
		std::unique_ptr<Scene> s(EntityFactory::get_inst().instantiate<Scene>(_resource_name));

		return LoadScene(std::move(s));
	}

}
