#pragma once
#include <Engine/Util/Singleton.h>

#include <Engine/Game/Scene.h>

namespace core
{
	class SceneManager
	{
		DECLARE_SINGLETON(SceneManager);
	private:
		SceneManager();
		void init();

		~SceneManager();

	public:
		void fixed_update();
		void update();
		void final_update();
		void frame_end();

		void destroy();

		s_ptr<Scene>	load_scene(const s_ptr<Scene>& _scene);
		s_ptr<Scene>	load_scene(const std::string_view _scene_name);
		s_ptr<Scene> get_active_scene() { return m_active_scene; }

	private:
		s_ptr<Scene>				m_active_scene;

		bool m_b_fixed_update;
	};
}
