
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

	void iScene::SceneFrameEnd()
	{
		while (false == m_FrameEndJobs.empty())
		{
			m_FrameEndJobs.front()();
			m_FrameEndJobs.pop();
		}
	}


	void iScene::AddGameObjects(const eLayerType _type, const std::vector<std::shared_ptr<GameObject>>& _gameObjects)
	{
		for (size_t i = 0; i < _gameObjects.size(); ++i)
		{
			m_Layers[(int)_type].AddGameObject(_gameObjects[i]);
		}
	}




	std::vector<std::shared_ptr<GameObject>> iScene::GetDontDestroyGameObjects()
	{
		std::vector<std::shared_ptr<GameObject>> dontGameObjs{};

		for (int i = 0; i < (int)eLayerType::END; ++i)
		{
			m_Layers[i].GetDontDestroyGameObjects(dontGameObjs);
		}

		return dontGameObjs;
	}
	const std::vector<std::shared_ptr<GameObject>>& iScene::GetGameObjects(const eLayerType _type)
	{
		return m_Layers[(uint)_type].GetGameObjects();
	}
}
