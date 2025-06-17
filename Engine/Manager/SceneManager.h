#pragma once
#include "Engine/Util/Singleton.h"

#include "Engine/Game/Scene.h"

namespace core
{
	class SceneManager
	{
		DECLARE_SINGLETON(SceneManager);
	private:
		SceneManager();
		~SceneManager();

	public:
		void fixed_update();
		void update();
		void final_update();
		void frame_end();

		void Destroy();

		s_ptr<Scene>  LoadScene(const s_ptr<Scene>& _scene);
		s_ptr<Scene>	LoadScene(const std::string_view _scene_name);
		s_ptr<Scene> GetActiveScene() { return m_activeScene; }

	private:
		s_ptr<Scene>				m_activeScene;

		bool m_isFixedUpdating;
	};
}
