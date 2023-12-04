#pragma once
#include "GameObject.h"

namespace ehw
{
	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		void Awake();
		void Update();
		void InternalUpdate();
		void Render();
		void Destroy();

		IScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(IScene* _scene) { m_OwnerScene = _scene; }
		void AddGameObject(GameObject* gameObject);
		void RemoveGameObject(const GameObject* gameObject);
		const std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }
		std::vector<GameObject*> GetDontDestroyGameObjects();

		void SetLayerType(eLayerType _type) { m_LayerType = _type; }

	private:
		IScene* m_OwnerScene;
		eLayerType m_LayerType;
		std::vector<GameObject*> mGameObjects;
	};

	typedef const std::vector<GameObject*>& GameObjects;
	typedef std::vector<GameObject*>::iterator GameObjectIter;
}
