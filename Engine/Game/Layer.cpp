#include "Engine/Game/Layer.h"



#include "Engine/Manager/RenderManager.h"


#include "Engine/Game/iScene.h"
#include "Engine/Game/GameObject.h"


namespace ehw
{
	Layer::Layer()
		: m_OwnerScene()
		, m_LayerType()
		, m_gameObjects{}
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Awake()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Awake();
		}
	}

	void Layer::Update()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->Update();
		}
	}

	void Layer::FinalUpdate()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			m_gameObjects[i]->FinalUpdate();
		}

		// sort z axis
		//std::vector<GameObject*> m_gameObjects;
		//std::sort(m_gameObjects.begin(), m_gameObjects.end(), CompareGameObjectByZAxis);
	}

	void Layer::Render()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			if (m_gameObjects[i]->IsActive())
			{
				m_gameObjects[i]->Render();
			}
		}
	}

	void Layer::FrameEnd()
	{
		for (size_t i = 0; i < m_gameObjects.size(); ++i)
		{
			if (m_gameObjects[i]->IsActive())
			{
				m_gameObjects[i]->FrameEnd();
			}
		}
	}

	void Layer::Destroy()
	{
		std::erase_if(m_gameObjects,
			[](const std::shared_ptr<GameObject>& _gameObj)
			{
				return _gameObj->IsDestroyed();
			}
		);
	}

	void Layer::AddGameObject(const std::shared_ptr<GameObject>& _gameObject)
	{
		m_gameObjects.push_back(_gameObject);

		_gameObject->SetOwnerScene(m_OwnerScene);
		_gameObject->SetLayer(m_LayerType);

		if (m_OwnerScene->IsAwaken())
		{
			_gameObject->Awake();
		}
	}

	void Layer::RemoveGameObject(const GameObject* gameObject)
	{
		if (gameObject)
		{
			auto iter = m_gameObjects.begin();
			const auto& iterEnd = m_gameObjects.end();
			for (iter; iter != iterEnd; ++iter)
			{
				if (gameObject == iter->get())
				{
					m_gameObjects.erase(iter);
					break;
				}
			}
		}
	}

	void Layer::GetDontDestroyGameObjects(std::vector<std::shared_ptr<GameObject>>& _dontObjects)
	{
		auto iter = std::partition(m_gameObjects.begin(), m_gameObjects.end(),
			[](const std::shared_ptr<GameObject>& _gameObj)->bool
			{
				return !_gameObj->IsDontDestroyOnLoad();
			}
		);
		
		std::move(iter, m_gameObjects.end(), std::back_inserter(_dontObjects));
	}
}
