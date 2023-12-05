#include "PCH_Engine.h"

#include "iScene.h"


namespace ehw
{
	iScene::iScene()
		: m_bAwake(false)
	{
		for (size_t i = 0; i < m_Layers.size(); ++i)
		{
			m_Layers[i].SetOwnerScene(this);
			m_Layers[i].SetLayerType((eLayerType)i);
		}
	}

	iScene::~iScene()
	{
	}

	void iScene::SceneAwake()
	{
		if (m_bAwake)
			return;

		m_bAwake = true;
		
		for (Layer& layer : m_Layers)
		{
			layer.Awake();
		}
	}
	void iScene::SceneUpdate()
	{
		Update();

		for (Layer& layer : m_Layers)
		{
			layer.Update();
		}
	}
	void iScene::SceneInternalUpdate()
	{
		InternalUpdate();
		for (Layer& layer : m_Layers)
		{
			layer.InternalUpdate();
		}
	}
	void iScene::SceneRender()
	{
		Render();
		for (Layer& layer : m_Layers)
		{
			layer.Render();
		}
	}
	void iScene::SceneDestroy()
	{
		Destroy();
		for (Layer& layer : m_Layers)
		{
			layer.Destroy();
		}
	}





	std::vector<GameObject*> iScene::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> gameObjects;
		for (Layer& layer : m_Layers)
		{
			std::vector<GameObject*> dontGameObjs
				= layer.GetDontDestroyGameObjects();

			gameObjects.insert(gameObjects.end()
			, dontGameObjs.begin()
			, dontGameObjs.end());
		}

		return gameObjects;
	}
	const std::vector<GameObject*>& iScene::GetGameObjects(const eLayerType _type)
	{
		return m_Layers[(uint)_type].GetGameObjects();
	}
}
