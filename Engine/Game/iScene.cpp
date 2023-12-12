#include "../PCH_Engine.h"
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

	GameObject* iScene::AddNewGameObjectHierarchy(const eLayerType _type, const std::shared_ptr<GameObject>& _newGameObj)
	{
		GameObject* ret = _newGameObj.get();

		ASSERT_DEBUG(ret, "GameObject가 nullptr이었습니다.");
	
		ret->SetLayerType(_type);

		auto Func = [this, _type, newGameObj = _newGameObj]()->void
			{
				//게임오브젝트 계층구조를 싹 받아온다.
				std::vector<std::shared_ptr<GameObject>> gameObjects = newGameObj->GetGameObjectsInHierarchy();
				for (size_t i = 0; i < gameObjects.size(); ++i)
				{
					ASSERT_DEBUG(false == gameObjects[i]->IsInScene(), "이미 Scene에 들어간 오브젝트는 추가할 수 없습니다.\n(새 게임 오브젝트만 추가 가능)");
					gameObjects[i]->SetOwnerScene(this);

					eLayerType presetLayerType = gameObjects[i]->GetLayerType();
					if (eLayerType::None == presetLayerType)
					{
						m_Layers[(int)_type].AddGameObject(gameObjects[i]);
					}
					else
					{
						m_Layers[(int)presetLayerType].AddGameObject(gameObjects[i]);
					}
				}
			};

		//아직 씬이 작동 전일 경우 바로 추가
		if (false == m_bAwake)
		{
			Func();
		}

		//씬이 작동 중일 경우 지연 추가
		else
		{
			AddFrameEndJob(Func);
		}

		return ret;
	}
}
