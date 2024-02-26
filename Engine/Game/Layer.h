#pragma once
#include "../Entity.h"


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
		void InternalUpdate();
		void Render();
		void FrameEnd();
		void Destroy();

		iScene* GetOwnerScene() const { return m_OwnerScene; }
		void SetOwnerScene(iScene* _scene) { m_OwnerScene = _scene; }
		void AddGameObject(const std::shared_ptr<GameObject>& _gameObject);

		void RemoveGameObject(const GameObject* gameObject);

		const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() { return m_GameObjects; }

		void GetDontDestroyGameObjects(std::vector<std::shared_ptr<GameObject>>& _dontObjects);

		void SetLayerType(eLayerType _type) { m_LayerType = _type; }

	private:
		iScene* m_OwnerScene;
		eLayerType m_LayerType;

		std::vector<std::shared_ptr<GameObject>> m_GameObjects;
	};
}
