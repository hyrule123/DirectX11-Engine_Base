
#include "Scene.h"
#include <Engine/Game/GameObject.h>

#include <Engine/Game/Collision/CollisionSystem.h>

namespace core
{
	Scene::Scene()
		: Super()
		, m_gameObjects{}
		, m_delayedAddQueue{}
		, m_FrameEndJobs{}
		, m_bAwake{ false }
		, m_layerNames{}
		, m_collisionSystem{}
	{
		m_layerNames[0] = "Default";
	}

	Scene::~Scene()
	{
		m_gameObjects.clear();
		m_delayedAddQueue.clear();
		m_collisionSystem.reset();
	}

	void Scene::init()
	{
		Super::init();
		m_collisionSystem = std::make_unique<CollisionSystem>(this);
	}

	void Scene::on_enter()
	{
	}

	void Scene::awake()
	{
		if (m_bAwake)
		{
			return;
		}

		m_bAwake = true;

		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->awake();
		}
	}

	void Scene::fixed_update()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->fixed_update();
		}

		if (m_collisionSystem)
		{
			m_collisionSystem->fixed_update();
		}
	}

	void Scene::update()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			if (m_gameObjects[i]->is_active())
			{
				m_gameObjects[i]->update();
			}
		}
	}

	void Scene::final_update()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			if (m_gameObjects[i]->is_active())
			{
				m_gameObjects[i]->final_update();
			}
		}
	}

	void Scene::frame_end()
	{
		//각 GameObject에 대해 frameend 호출
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->frame_end();
		}

		if (m_collisionSystem)
		{
			m_collisionSystem->frame_end();
		}

		//FrameEnd에 예약해놓은 함수 호출
		for (size_t i = 0; i < m_FrameEndJobs.size(); ++i)
		{
			m_FrameEndJobs[i]();
		}
		m_FrameEndJobs.clear();

		remove_destroyed();

		//마지막으로 Scene 작동 중 추가된 GameObject를 추가
		if (false == m_delayedAddQueue.empty()) 
		{
			for (const s_ptr<GameObject>& obj : m_delayedAddQueue)
			{
				add_game_object_recursive(obj);
			}

		}
	}

	void Scene::destroy()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->destroy();
		}
	}

	s_ptr<GameObject> Scene::add_game_object_recursive(const s_ptr<GameObject>& _newObject)
	{
		s_ptr<GameObject> ret = nullptr;
		//true가 반환되면 에러 있다는 뜻
		if (validate_object(_newObject))
		{
			DEBUG_MESSAGE("Scene에 추가하려는 GameObject에 문제가 있어 추가하지 못했습니다.");
			return nullptr;
		}

		if (false == m_bAwake)
		{
			add_game_object_internal(_newObject);
		}

		//Scene이 재생 중일 경우에는 한 프레임이 끝난 후 추가한다.
		else
		{
			m_delayedAddQueue.push_back(_newObject);
		}

		return ret;
	}

	std::vector<s_ptr<GameObject>> Scene::add_game_objects(const std::vector<s_ptr<GameObject>>& _gameObjects)
	{
		std::vector<s_ptr<GameObject>> inserted;
		inserted.reserve(_gameObjects.size());

		for (const s_ptr<GameObject>& obj : _gameObjects)
		{
			s_ptr<GameObject> result = add_game_object_recursive(obj);
			if (result)
			{
				inserted.push_back(obj);
			}
		}

		return inserted;
	}

	std::vector<s_ptr<GameObject>> Scene::get_dont_destroy_game_objects()
	{
		std::vector<s_ptr<GameObject>> ret{};

		for (const s_ptr<GameObject>& obj : m_gameObjects)
		{
			if (obj->is_not_destroyed_on_load())
			{
				ret.push_back(obj);
			}
		}

		return ret;
	}

	void Scene::remove_destroyed()
	{
		//Destroy 상태의 Object 제거
		std::erase_if(m_gameObjects,
			[](const s_ptr<GameObject>& _obj)->bool
			{
				GameObject::eState state = _obj->get_state();
				if (GameObject::eState::destroy == state)
				{
					return true;
				}
				else
				{
					_obj->remove_destroyed();
				}

				return false;
			}
		);
	}

	bool Scene::validate_object(const s_ptr<GameObject>& _obj)
	{
		if (nullptr == _obj)
		{
			DEBUG_MESSAGE("object가 nullptr 입니다.");
			return false;
		}
		if (false == IsLayerValid(_obj->get_layer()))
		{
			DEBUG_MESSAGE("Layer 번호가 유효하지 않습니다.(0~31)");
			return false;
		}

		//이미 scene에 들어간 GameObject는 넣지 않는다.
		if (this == _obj->get_scene().get())
		{
			DEBUG_MESSAGE("이미 Scene에 들어간 GameObject 입니다.");
			return false;
		}

		_obj->set_scene(std::static_pointer_cast<Scene>(this->shared_from_this()));
		return true;
	}

	void Scene::add_game_object_internal(const s_ptr<GameObject>& _obj)
	{
		m_gameObjects.push_back(_obj);
		if (m_bAwake)
		{
			_obj->awake();
		}
	}
}
