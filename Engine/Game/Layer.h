#pragma once
#include "Engine/Entity.h"


namespace ehw
{
	class iScene;
	class GameObject;

	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		void Awake();
		void Update();
		void FinalUpdate();
		void Render();
		void FrameEnd();
		void Destroy();

		iScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(iScene* _scene) { m_OwnerScene = _scene; }
		void AddGameObject(const std::shared_ptr<GameObject>& _gameObject);

		void RemoveGameObject(const GameObject* gameObject);

		const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return m_gameObjects; }

		void GetDontDestroyGameObjects(std::vector<std::shared_ptr<GameObject>>& _dontObjects);

		void SetLayerType(uint32 _type) { m_LayerType = _type; }

	private:
		iScene* m_OwnerScene;

		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
		uint32 m_LayerType;

		
	};
}
