#include "PCH_Engine.h"

#include "Layer.h"
#include "IScene.h"

#include "RenderMgr.h"
#include "Com_Transform.h"



namespace ehw
{

	Layer::Layer()
		: m_OwnerScene()
		, m_LayerType()
		, mGameObjects{}
	{
	}

	Layer::~Layer()
	{
		auto iter = std::remove_if(mGameObjects.begin(), mGameObjects.end(),
			[](GameObject* _obj)->bool
			{
				return !(_obj->IsMaster());
			});
		mGameObjects.erase(iter, mGameObjects.end());
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			delete mGameObjects[i];
		}
	}

	void Layer::Awake()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster())
				mGameObjects[i]->Awake();
		}
	}

	void Layer::Update()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster())
				mGameObjects[i]->Update();
		}
	}

	void Layer::InternalUpdate()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsMaster())
				mGameObjects[i]->InternalUpdate();
		}

		// sort z axis
		//std::vector<GameObject*> mGameObjects;
		//std::sort(mGameObjects.begin(), mGameObjects.end(), CompareGameObjectByZAxis);
	}

	void Layer::Render()
	{
		for (size_t i = 0; i < mGameObjects.size(); ++i)
		{
			if (mGameObjects[i]->IsActive())
				mGameObjects[i]->Render();
		}
	}

	void Layer::Destroy()
	{
		std::vector<GameObject*> masterObjects;
		auto iter = std::remove_if(mGameObjects.begin(), mGameObjects.end(),
			[&](GameObject* _obj)->bool
			{
				bool bRet = false;
				if (_obj->IsDestroyed())
				{
					if (_obj->IsMaster())
						masterObjects.push_back(_obj);
					bRet = true;
				}
				return bRet;
			}
		);
		mGameObjects.erase(iter, mGameObjects.end());

		for (size_t i = 0; i < masterObjects.size(); ++i)
		{
			delete masterObjects[i];
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		ASSERT(gameObject, "GameObject가 nullptr 입니다.");

		mGameObjects.push_back(gameObject);
		gameObject->SetOwnerScene(m_OwnerScene);
		gameObject->SetLayerType(m_LayerType);

		if (m_OwnerScene->IsAwaken() && gameObject->IsActive())
		{
			gameObject->Awake();
		}
	}

	void Layer::RemoveGameObject(const GameObject* gameObject)
	{
		if (gameObject)
		{
			auto iter = mGameObjects.begin();
			const auto& iterEnd = mGameObjects.end();
			for (iter; iter != iterEnd; ++iter)
			{
				if (gameObject == (*iter))
				{
					mGameObjects.erase(iter);
					break;
				}
			}
		}
	}
	std::vector<GameObject*> Layer::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> donts;

		//mGameObjects에서 삭제 안할 게임오브젝트들을 제거하고 
		//그걸 donts 벡터에 추가
		//나머지는 냄겨둠
		mGameObjects.erase
		(
			std::remove_if(mGameObjects.begin(), mGameObjects.end(), 
				[&](GameObject* _obj)
				{
					bool result = false;
					if (_obj && _obj->IsDontDestroyOnSceneChange())
					{
						donts.push_back(_obj);
						result = true;
					}

					return result;
				})
			, mGameObjects.end()
		);

		return donts;
	}
}
