#include "Engine/Game/iScene.h"

#include "Engine/Game/GameObject.h"

namespace ehw
{
	iScene::iScene()
		: m_gameObjects()
		, m_FrameEndJobs()
		, m_bAwake(false)
	{
		m_layerNames[0] = "Default";
	}

	iScene::~iScene()
	{
	}

	void iScene::SceneAwake()
	{
		if (m_bAwake)
		{
			return;
		}

		m_bAwake = true;
		
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Awake();
		}
	}
	void iScene::SceneUpdate()
	{
		Update();


		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Update();
		}
	}

	void iScene::SceneCollisionUpdate()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->CollisionUpdate();
		}
	}

	void iScene::SceneFinalUpdate()
	{
		FinalUpdate();
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FinalUpdate();
		}
	}
	void iScene::SceneRender()
	{
		Render();
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Render();
		}
	}
	void iScene::SceneRemoveDestroyed()
	{
		RemoveDestroyed();

		//Destroy 상태의 Object 제거, 아닐 경우 RemoveDestroyed 함수 호출(컴포넌트 지울거있는지 확인)
		std::erase_if(m_gameObjects, 
			[](const std::shared_ptr<GameObject>& _obj)->bool
			{
				GameObject::eState state = _obj->GetState();
				if (GameObject::eState::Destroy == state)
				{
					return true;
				}
				else
				{
					_obj->RemoveDestroyed();
				}

				return false;
			}
		);
	}

	void iScene::SceneFrameEnd()
	{
		FrameEnd();

		//각 GameObject에 대해 frameend 호출
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FrameEnd();
		}

		//FrameEnd에 예약해놓은 함수 호출
		while (false == m_FrameEndJobs.empty())
		{
			m_FrameEndJobs.front()();
			m_FrameEndJobs.pop();
		}

		//마지막으로 Scene 작동 중 추가된 GameObject를 추가
		AddGameObjectsInternal(m_delayedAddQueue);
	}


	std::shared_ptr<GameObject> iScene::AddGameObject(const std::shared_ptr<GameObject>& _gameObject, const uint _layer)
	{
		//true가 반환되면 에러 있다는 뜻
		if (SetGameObjectInfo(_gameObject, _layer))
		{
			return nullptr;
		}

		
		if (false == m_bAwake)
		{
			AddGameObjectInternal(_gameObject);
		}

		//Scene이 재생 중일 경우에는 한 프레임이 끝난 후 추가한다.
		else
		{
			m_delayedAddQueue.push_back(_gameObject);
		}

		return _gameObject;
	}

	size_t iScene::AddGameObjects(const GameObjects& _gameObjects, const uint _layer)
	{
		size_t inserted = 0;

		//복사
		GameObjects gameObjects = _gameObjects;

		std::function<bool(const std::shared_ptr<GameObject>&)> predFunc =
			std::bind(&iScene::SetGameObjectInfo, this, std::placeholders::_1, _layer);
		
		std::erase_if(gameObjects, predFunc);

		inserted = gameObjects.size();

		if (m_bAwake)
		{
			m_delayedAddQueue.insert(m_delayedAddQueue.end()
				, std::make_move_iterator(gameObjects.begin())
				, std::make_move_iterator(gameObjects.end())
			);
		}
		else
		{
			AddGameObjectsInternal(gameObjects);
		}

		return inserted;
	}

	std::vector<std::shared_ptr<GameObject>> iScene::GetDontDestroyGameObjects()
	{
		GameObjects dontGameObjs{};
		dontGameObjs.reserve(m_gameObjects.size());

		auto iter = std::partition(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::shared_ptr<GameObject>& _gameObj)->bool
			{
				return !_gameObj->IsDontDestroyOnLoad();
			}
		);

		dontGameObjs.insert(dontGameObjs.end(), std::make_move_iterator(iter), std::make_move_iterator(m_gameObjects.end()));

		return dontGameObjs;
	}

	bool iScene::SetGameObjectInfo(const std::shared_ptr<GameObject>& _obj, const uint _layer)
	{
		if (nullptr == _obj)
		{
			return true;
		}
		else if (false == IsLayerValid(_layer))
		{
			return true;
		}

		//레이어는 일단 변경시켜줌
		_obj->SetLayer(_layer);

		//OwnerScene이 이미 this가 되었다는건 이미 m_gameObjects에 들어왔다는 뜻
		if (this == _obj->GetOwnerScene())
		{
			return true;
		}
		_obj->SetOwnerScene(this);

		return false;
	}

	void iScene::AddGameObjectInternal(const std::shared_ptr<GameObject>& _obj)
	{
		m_gameObjects.push_back(_obj);

		if (m_bAwake)
		{
			_obj->Awake();
		}
	}

	void iScene::AddGameObjectsInternal(GameObjects& _from)
	{
		auto iter = m_gameObjects.insert(m_gameObjects.end()
			, std::make_move_iterator(_from.begin())
			, std::make_move_iterator(_from.end())
		);
		_from.clear();

		for (iter; iter != m_gameObjects.end(); ++iter)
		{
			if (m_bAwake)
			{
				(*iter)->Awake();
			}
		}
	}
}
