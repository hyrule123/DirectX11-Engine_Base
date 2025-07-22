#pragma once
#include <Engine/Game/Entity.h>
#include <Engine/Common.h>
#include <Engine/Util/type_traits_Ex.h>

namespace core
{
	class GameObject;
	class CollisionSystem;
	class Scene 
		: public Entity
	{
		CLASS_INFO(Scene, Entity);
	public:
		Scene();
		virtual ~Scene();

		virtual void init();
		virtual void awake();
		virtual void on_enter();

		virtual void fixed_update();
		virtual void update();
		virtual void final_update();
		virtual void frame_end();

		void destroy();

		bool	is_awaken() const { return m_bAwake; }

		s_ptr<GameObject> add_game_object_recursive(const s_ptr<GameObject>& _newObject);

		//들어간 갯수
		std::vector<s_ptr<GameObject>> add_game_objects(const std::vector<s_ptr<GameObject>>& _gameObjects);

		std::vector<s_ptr<GameObject>>		get_dont_destroy_game_objects();
		const std::vector<s_ptr<GameObject>>& get_game_objects() { return m_gameObjects; }

		//있음이 보장됨 - 전방 선언
		CollisionSystem* get_collision_system() { return m_collisionSystem.get(); }

		template <class F, class... Args>
		inline void add_frame_end_job(F&& _func, Args&&... _args);

	private:
		void remove_destroyed();

		//true: 문제 발생, false: 문제 없음 - stl pred func에 필요함
		bool validate_object(const s_ptr<GameObject>& _obj);
		void add_game_object_internal(const s_ptr<GameObject>& _obj);
		
	private:
		std::vector<s_ptr<GameObject>> m_gameObjects;

		//Scene 진행 중일 경우에는 여기에 임시로 넣었다가 씬에 추가함
		std::vector<s_ptr<GameObject>> m_delayedAddQueue;

		std::vector<std::function<void()>> m_FrameEndJobs;

		bool m_bAwake;

		std::array<std::string, g_maxLayer>				m_layerNames;

		std::unique_ptr<CollisionSystem> m_collisionSystem;
	};


	//템플릿의 &&는 상황에 따라서 맞춰서 전달된다.
	template<class F, class ...Args>
	inline void Scene::add_frame_end_job(F&& _func, Args && ..._args)
	{
		//forward: 들어온 인자의 레퍼런스에 맞는 타입으로 전달
		m_FrameEndJobs.push_back(std::bind(std::forward<F>(_func), std::forward<Args>(_args)...));
	}
}
