#pragma once
#include "Engine/Entity.h"

#include "Engine/Util/type_traits_Ex.h"

#include "Engine/define_Enum.h"

#include <queue>
#include <future>
#include <bitset>

namespace ehw
{
	class GameObject;
	class iScene 
		: public Entity
	{
	public:
		//GameObjects를 std::vector<GameObject>로 만든다면?
		//매 프레임마다 partition을 하는데, partition 발생시 이동 할당 연산자 호출됨.
		//	-> 모든 컴포넌트를 순회하며 SetOwner(this) 호출함.
		
		//GameObjects를 std::list<GameObject>로 만든다면?
		//힙 여기저기 널부러진 GameObject 인스턴스를 찾아 업데이트 하는 셈.
		//이러면 사실상 std::vector<GameObject*> 와 다를 게 없다.

		//std::vector<std::shared_ptr<GameObject>>를 사용 시
		//이 때는 람다 함수에 GameObject가 남아있는 채로 종료될 시 메모리 해제를 해줄수 있는 방법이 없다.
		//-> 일반적인 방법으로 게임 종료시 람다함수를 반드시 실행시키도록 설정

		using GameObjects = std::vector<std::shared_ptr<GameObject>>;

		iScene();
		virtual ~iScene();

		//로직 호출
		void SceneAwake();
		void SceneUpdate();
		void SceneCollisionUpdate();
		void SceneFinalUpdate();
		void SceneRender();
		void SceneRemoveDestroyed();
		void SceneFrameEnd();


		//로직 재정의
		virtual void OnEnter() = 0;//리소스 로드

		virtual void Update() {}
		virtual void FinalUpdate() {}
		virtual void Render() {}
		virtual void RemoveDestroyed() {}
		virtual void FrameEnd() {}
		virtual void OnExit() {}

		bool	IsAwaken() const { return m_bAwake; }

		std::shared_ptr<GameObject> AddGameObject(const std::shared_ptr<GameObject>& _gameObject, const uint _layer);

		//들어간 갯수
		size_t AddGameObjects(const GameObjects& _gameObjects, const uint _layer);

		GameObjects		GetDontDestroyGameObjects();
		const GameObjects& GetGameObjects() { return m_gameObjects; }

		template <class F, class... Args>
		inline void AddFrameEndJob(F&& _func, Args&&... _args);

	private:
		//true: 문제 있음, false: 문제 없음
		bool SetGameObjectInfo(const std::shared_ptr<GameObject>& _obj, const uint _layer);
		void AddGameObjectInternal(const std::shared_ptr<GameObject>& _obj);
		void AddGameObjectsInternal(GameObjects& _from);
		
	private:
		GameObjects m_gameObjects;

		//Scene 진행 중일 경우에는 여기에 임시로 넣었다가 씬에 추가함
		GameObjects m_delayedAddQueue;

		std::array<std::string, g_maxLayer>				m_layerNames;

		std::queue<std::function<void()>> m_FrameEndJobs;

		bool m_bAwake;
	};



	template<class F, class ...Args>
	inline void iScene::AddFrameEndJob(F&& _func, Args && ..._args)
	{
		m_FrameEndJobs.push(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
	}
}
