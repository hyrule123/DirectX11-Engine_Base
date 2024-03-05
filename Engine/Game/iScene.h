#pragma once
#include "Entity.h"



#include "Util/SimpleMath.h"


#include "Layer.h"

#include "Game/GameObject.h"


#include <queue>
#include <future>

namespace ehw
{
	class iScene 
		: public Entity
	{
	public:
		iScene();
		virtual ~iScene();

		//실제 로직은 여기서
		void SceneAwake();
		void SceneUpdate();
		void SceneInternalUpdate();
		void SceneRender();
		void SceneDestroy();
		void SceneFrameEnd();


		//아래 함수를 재정의해서 씬을 커스터마이즈 하면 됨
		virtual void OnEnter() = 0;//리소스 로드

		virtual void Update() {}
		virtual void InternalUpdate() {}
		virtual void Render() {}
		virtual void Destroy() {}
		virtual void FrameEnd() {}
		virtual void OnExit() {}
		
		Layer&							GetLayer(eLayer _type) { return m_Layers[(uint)_type]; }

		bool	IsAwaken() const { return m_bAwake; }

		//Add 'New' Game Object -> 이미 레이어에 들어갔었던 게임오브젝트는 이 함수를 사용하면 안됨
		inline std::shared_ptr<GameObject> NewGameObject(const eLayer _type);
		inline std::shared_ptr<GameObject> NewGameObject(const eLayer _type, const std::string_view _name);
		void AddGameObjects(const eLayer _type, const std::vector<std::shared_ptr<GameObject>>& _gameObjects);
		inline void AddGameObject(const eLayer _type, const std::shared_ptr<GameObject>& _gameObject);

		std::vector<std::shared_ptr<GameObject>>		GetDontDestroyGameObjects();
		const std::vector<std::shared_ptr<GameObject>>& GetGameObjects(const eLayer _type);

		template <class F, class... Args>
		inline void AddFrameEndJob(F&& _func, Args&&... _args);

		//리턴값이 필요한 경우(future 변수 사용)
		template <class F, class... Args>
		inline std::future<typename std::invoke_result<F, Args...>::type> 
			AddFrameEndJobReturn(F&& _func, Args&&... _args);

	private:
		std::array<Layer, (int)eLayer::END> m_Layers;
		bool m_bAwake;

		std::queue<std::function<void()>> m_FrameEndJobs;
	};

	inline std::shared_ptr<GameObject> iScene::NewGameObject(const eLayer _type)
	{
		std::shared_ptr retObj = std::make_shared<GameObject>();
		m_Layers[(int)_type].AddGameObject(retObj);
		return retObj;
	}

	inline std::shared_ptr<GameObject> iScene::NewGameObject(const eLayer _type, const std::string_view _name)
	{
		std::shared_ptr retObj = std::make_shared<GameObject>();
		m_Layers[(int)_type].AddGameObject(retObj);
		retObj->SetName(_name);
		return retObj;
	}

	inline void iScene::AddGameObject(const eLayer _type, const std::shared_ptr<GameObject>& _gameObject)
	{
		m_Layers[(int)_type].AddGameObject(_gameObject);
	}

	template<class F, class ...Args>
	inline void iScene::AddFrameEndJob(F&& _func, Args && ..._args)
	{
		m_FrameEndJobs.push(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
	}


	template<class F, class ...Args>
	inline std::future<typename std::invoke_result<F, Args...>::type> iScene::AddFrameEndJobReturn(F&& _func, Args && ..._args)
	{
		using return_type = std::invoke_result<F, Args...>::type;

		auto pFunc = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(_func), std::forward<Args>(_args)...)
		);

		std::future<return_type> pFunc_result_future = pFunc->get_future();
		m_FrameEndJobs.push([pFunc]() { (*pFunc)(); });

		return pFunc_result_future;
	}

}
