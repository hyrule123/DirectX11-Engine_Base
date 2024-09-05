#pragma once
#include "Engine/Util/StaticSingleton.h"

#include "Engine/Game/Scene.h"

namespace ehw
{
	class SceneManager : public StaticSingleton<SceneManager>
	{
		friend class StaticSingleton<SceneManager>;
	private:
		SceneManager();
		~SceneManager();

	public:
		void Init();
		void FixedUpdate();
		void Update();
		void final_update();
		void frame_end();
		void Release();

		void Destroy();

		Scene*  LoadScene(std::unique_ptr<Scene> _scene);
		Scene*	LoadScene(const std::string_view _strKey);
		Scene* GetActiveScene() { return m_activeScene; }

	private:
		Scene*				m_activeScene;

		bool m_isFixedUpdating;
	};
}
