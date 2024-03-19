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
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FrameEnd();
		}

		while (false == m_FrameEndJobs.empty())
		{
			m_FrameEndJobs.front()();
			m_FrameEndJobs.pop();
		}
	}

	std::shared_ptr<GameObject> iScene::NewGameObject(const uint _layer, const std::string_view _name)
	{
		return AddGameObject(std::make_shared<GameObject>(), _layer, _name);
	}

	std::shared_ptr<GameObject> iScene::AddGameObject(const std::shared_ptr<GameObject>& _gameObject, const uint _layer, const std::string_view _name)
	{
		//게임오브젝트가 nullptr 이면 그대로 nullptr 반환
		if (nullptr == _gameObject)
		{
			return nullptr;
		}
		else if (false == CheckLayerValid(_layer))
		{
			return nullptr;
		}

		//레이어와 이름 바꿔준다.
		_gameObject->SetLayer(_layer);
		_gameObject->SetName(_name);

		//이미 자신의 scene에 들어가 있다면 그대로 반환
		if (this == _gameObject->GetOwnerScene())
		{
			return _gameObject;
		}

		_gameObject->SetOwnerScene(this);


		auto lambda = 
			[this, _obj = _gameObject]()->void
			{
				m_gameObjects.push_back(_obj);

				if (m_bAwake)
				{
					_obj->Awake();
				}
			};

		//Scene이 재생 중일 경우에는 프레임 재생이 끝난 후 추가한다.
		if (m_bAwake)
		{
			AddFrameEndJob(lambda);
		}
		else
		{
			lambda();
		}

		return _gameObject;
	}

	std::vector<std::shared_ptr<GameObject>> iScene::GetDontDestroyGameObjects()
	{
		std::vector<std::shared_ptr<GameObject>> dontGameObjs{};
		dontGameObjs.reserve(m_gameObjects.size());

		auto iter = std::partition(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::shared_ptr<GameObject>& _gameObj)->bool
			{
				return !_gameObj->IsDontDestroyOnLoad();
			}
		);

		std::move(iter, m_gameObjects.end(), std::back_inserter(dontGameObjs));

		return dontGameObjs;
	}
}
