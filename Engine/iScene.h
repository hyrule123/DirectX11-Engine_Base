#pragma once
#include "Entity.h"
#include "Layer.h"

namespace ehw
{
	class GameObject;
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

		virtual void Update() {};
		virtual void InternalUpdate() {};
		virtual void Render() {};
		virtual void Destroy() {};
		virtual void OnExit() {};
		
		Layer&							GetLayer(eLayerType _type) { return m_Layers[(uint)_type]; }

		bool	IsAwaken() const { return m_bAwake; }

		//Add 'New' Game Object -> 이미 레이어에 들어갔었던 게임오브젝트는 이 함수를 사용하면 안됨
		inline GameObject* NewGameObject(const eLayerType _type);
		GameObject* AddNewGameObject(const eLayerType _type, std::shared_ptr<GameObject> _newGameObj);

		template <class F, class... Args>
		inline void AddFrameEndJob(F&& _func, Args&&... _args);

		//리턴값이 필요한 경우(future 변수 사용)
		template <class F, class... Args>
		inline std::future<typename std::invoke_result<F, Args...>::type> 
			AddFrameEndJobReturn(F&& _func, Args&&... _args);

		std::vector<std::shared_ptr<GameObject>>		GetDontDestroyGameObjects();
		const std::vector<std::shared_ptr<GameObject>>& GetGameObjects(const eLayerType _type);

	private:
		std::array<Layer, (int)eLayerType::END> m_Layers;
		bool m_bAwake;

		std::queue<std::function<void()>> m_FrameEndJobs;
	};

	inline GameObject* iScene::NewGameObject(const eLayerType _type)
	{
		return AddNewGameObject(_type, std::make_shared<GameObject>());
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
