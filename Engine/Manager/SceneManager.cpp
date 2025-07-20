#include "ResourceManagers.h"
#include "SceneManager.h"

#include <Engine/Manager/ResourceManagers.h>

#include <Engine/Resource/Texture.h>

#include <Engine/Util/AtExit.h>

#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/TimeManager.h>

#include <chrono>

namespace core
{
	SceneManager::SceneManager()
		: m_active_scene{nullptr}
		, m_b_fixed_update{false}
	{}
	void SceneManager::init()
	{
		m_active_scene = nullptr;

		AtExit::add_func(SceneManager::destroy_inst);
	}

	SceneManager::~SceneManager()
	{
	}

	void SceneManager::fixed_update()
	{
		if (m_active_scene)
		{
			TimeManager::get_inst().set_fixed_update_mode(true);

			uint fixedUpdateCount = TimeManager::get_inst().get_fixed_update_count();

			for(uint i = 0; i < fixedUpdateCount; ++i)
			{
				m_active_scene->fixed_update();
			}

			TimeManager::get_inst().set_fixed_update_mode(false);
		}
	}

	void SceneManager::update()
	{
		if (m_active_scene)
		{
			m_active_scene->update();
		}
	}


	void SceneManager::final_update()
	{
		if (m_active_scene)
		{
			m_active_scene->final_update();
		}
	}

	void SceneManager::frame_end()
	{
		if (m_active_scene)
		{
			m_active_scene->frame_end();
		}
	}

	void SceneManager::destroy()
	{
		if (m_active_scene)
		{
			m_active_scene->destroy();
		}
	}

	s_ptr<Scene> SceneManager::load_scene(const s_ptr<Scene>& _scene)
	{
		ASSERT(nullptr != _scene, "scene이 nullptr 입니다.");

			// 바뀔때 dontDestory 오브젝트는 다음씬으로 같이 넘겨줘야한다.
			//ASSERT(false, "미구현");
			//std::vector<s_ptr<GameObject>> dontDestroyObjs;

			//if (m_active_scene)
			//{
			//	dontDestroyObjs = m_active_scene->get_dont_destroy_game_objects();
			//}

			//if (m_active_scene)
			//	m_active_scene->OnExit();

		//씬 갈아끼우기
		m_active_scene = _scene;

		//ASSERT(false, "미구현");
		//for (size_t i = 0; i < dontDestroyObjs.size(); ++i)
		//{
		//	
		//	m_active_scene->add_game_object_recursive(dontDestroyObjs[i]->GetLayerType(), dontDestroyObjs[i]);
		//}

		//OnEnter 호출
		m_active_scene->init();
		m_active_scene->on_enter();
		m_active_scene->awake();

		return m_active_scene;
	}

	s_ptr<Scene> SceneManager::load_scene(const std::string_view _resource_name)
	{
		s_ptr<Scene> loaded = 
			std::dynamic_pointer_cast<Scene>(EntityFactory::get_inst().instantiate(_resource_name));
		return load_scene(loaded);
	}

}
