#include "PCH_Engine.h"

#include "IScene.h"


namespace ehw
{
	IScene::IScene()
		: m_bAwake(false)
	{
		for (size_t i = 0; i < m_Layers.size(); ++i)
		{
			m_Layers[i].SetLayerType((eLayerType)i);
		}
	}

	IScene::~IScene()
	{
	}

	void IScene::SceneAwake()
	{
		if (m_bAwake)
			return;

		m_bAwake = true;
		
		for (Layer& layer : m_Layers)
		{
			layer.Awake();
		}
	}
	void IScene::SceneUpdate()
	{
		Update();

		for (Layer& layer : m_Layers)
		{
			layer.Update();
		}
	}
	void IScene::SceneFixedUpdate()
	{
		FixedUpdate();
		for (Layer& layer : m_Layers)
		{
			layer.FixedUpdate();
		}
	}
	void IScene::SceneRender()
	{
		Render();
		for (Layer& layer : m_Layers)
		{
			layer.Render();
		}
	}
	void IScene::SceneDestroy()
	{
		Destroy();
		for (Layer& layer : m_Layers)
		{
			layer.Destroy();
		}
	}





	std::vector<GameObject*> IScene::GetDontDestroyGameObjects()
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
	const std::vector<GameObject*>& IScene::GetGameObjects(const eLayerType _type)
	{
		return m_Layers[(uint)_type].GetGameObjects();
	}
}
