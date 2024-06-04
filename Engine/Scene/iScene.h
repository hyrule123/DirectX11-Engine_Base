#pragma once
#include "Engine/Entity.h"
#include "Engine/Common.h"
#include "Engine/Util/type_traits_Ex.h"

namespace ehw
{
	class GameObject;
	class CollisionSystem;
	class SceneRenderer;
	class iScene 
		: public Entity
	{
	public:
		iScene();
		virtual ~iScene();

		void SceneInit();
		void SceneAwake();
		void SceneFixedUpdate();
		void SceneUpdate();
		void SceneFinalUpdate();
		void SceneRender();
		void SceneFrameEnd();

		//로직 재정의
		virtual void Init() {}
		virtual void OnEnter() = 0;//리소스 로드

		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void FinalUpdate() {}
		virtual void Render() {}
		virtual void FrameEnd() {}

		void Destroy();

		bool	IsAwaken() const { return m_bAwake; }

		GameObject* AddGameObject(std::unique_ptr<GameObject>& _newObject, const uint _layer);

		//들어간 갯수
		size_t AddGameObjects(GameObjects& _gameObjects, const uint _layer);

		GameObjects		GetDontDestroyGameObjects();
		const GameObjects& GetGameObjects() { return m_gameObjects; }

		CollisionSystem* GetCollisionSystem() { 
			if (nullptr == m_collisionSystem) { CreateCollisionSystem(); }
			return m_collisionSystem.get(); 
		}

		SceneRenderer* GetSceneRenderer() {
			if(nullptr == m_sceneRenderer) { CreateSceneRenderer(); }
			return m_sceneRenderer.get();
		}

		template <class F, class... Args>
		inline void AddFrameEndJob(F&& _func, Args&&... _args);

	private:
		void CreateCollisionSystem();
		void CreateSceneRenderer();
		void RemoveDestroyed();

		//true: 문제 발생, false: 문제 없음 - stl pred func에 필요함
		bool SetGameObjectInfo(std::unique_ptr<GameObject>& _obj, const uint _layer);
		void AddGameObjectInternal(std::unique_ptr<GameObject>& _obj);
		void AddGameObjectsInternal(GameObjects& _from);
		
	private:
		GameObjects m_gameObjects;

		//Scene 진행 중일 경우에는 여기에 임시로 넣었다가 씬에 추가함
		GameObjects m_delayedAddQueue;

		std::vector<std::function<void()>> m_FrameEndJobs;

		bool m_bAwake;

		std::array<std::string, g_maxLayer>				m_layerNames;

		std::unique_ptr<CollisionSystem> m_collisionSystem;
		std::unique_ptr<SceneRenderer> m_sceneRenderer;
	};


	//템플릿의 &&는 상황에 따라서 맞춰서 전달된다.
	template<class F, class ...Args>
	inline void iScene::AddFrameEndJob(F&& _func, Args && ..._args)
	{
		//forward: 들어온 인자의 레퍼런스에 맞는 타입으로 전달
		m_FrameEndJobs.push_back(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
	}
}
