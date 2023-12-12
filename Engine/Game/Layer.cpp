#include "../PCH_Engine.h"
#include "Layer.h"



#include "../Manager/RenderManager.h"


#include "iScene.h"
#include "GameObject.h"


namespace ehw
{
	Layer::Layer()
		: m_OwnerScene()
		, m_LayerType()
		, m_GameObjects{}
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::Awake()
	{
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsMaster())
				m_GameObjects[i]->Awake();
		}
	}

	void Layer::Update()
	{
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsMaster())
				m_GameObjects[i]->Update();
		}
	}

	void Layer::InternalUpdate()
	{
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsMaster())
				m_GameObjects[i]->InternalUpdate();
		}

		// sort z axis
		//std::vector<GameObject*> m_GameObjects;
		//std::sort(m_GameObjects.begin(), m_GameObjects.end(), CompareGameObjectByZAxis);
	}

	void Layer::Render()
	{
		for (size_t i = 0; i < m_GameObjects.size(); ++i)
		{
			if (m_GameObjects[i]->IsActive())
				m_GameObjects[i]->Render();
		}
	}

	void Layer::Destroy()
	{
		std::erase_if(m_GameObjects,
			[](const std::shared_ptr<GameObject>& _gameObj)
			{
				return _gameObj->IsDestroyed();
			}
		);
	}

	void Layer::AddGameObject(const std::shared_ptr<GameObject>& _gameObject)
	{
		m_GameObjects.push_back(_gameObject);
		
		_gameObject->SetLayerType(m_LayerType);

		if (m_OwnerScene->IsAwaken())
		{
			_gameObject->Awake();
		}
	}

	void Layer::RemoveGameObject(const GameObject* gameObject)
	{
		if (gameObject)
		{
			auto iter = m_GameObjects.begin();
			const auto& iterEnd = m_GameObjects.end();
			for (iter; iter != iterEnd; ++iter)
			{
				if (gameObject == iter->get())
				{
					m_GameObjects.erase(iter);
					break;
				}
			}
		}
	}

	void Layer::GetDontDestroyGameObjects(std::vector<std::shared_ptr<GameObject>>& _dontObjects)
	{
		auto iter = std::partition(m_GameObjects.begin(), m_GameObjects.end(),
			[](const std::shared_ptr<GameObject>& _gameObj)->bool
			{
				return !_gameObj->IsDontDestroyOnLoad();
			}
		);
		
		std::move(iter, m_GameObjects.end(), std::back_inserter(_dontObjects));
	}
}
