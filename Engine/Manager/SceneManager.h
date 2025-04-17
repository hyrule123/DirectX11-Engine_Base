#pragma once
#include "Engine/Util/StaticSingleton.h"

#include "Engine/Game/Scene.h"

namespace core
{
	class SceneManager : public StaticSingleton<SceneManager>
	{
		friend class StaticSingleton<SceneManager>;
	private:
		SceneManager();
		~SceneManager();

	public:
		void fixed_update();
		void update();
		void final_update();
		void frame_end();

		void Destroy();

		Scene*  LoadScene(std::unique_ptr<Scene> _scene);
		Scene*	LoadScene(const std::string_view _resource_name);
		Scene* GetActiveScene() { return m_activeScene; }

	private:
		Scene*				m_activeScene;

		bool m_isFixedUpdating;
	};
}
