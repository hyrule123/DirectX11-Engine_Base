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
		void FinalUpdate();
		void FrameEnd();
		void Release();

		void Destroy();

		void		LoadScene(const std::string_view _strKey);
		Scene*		GetActiveScene() { return m_activeScene.get(); }

		template <typename T> requires std::is_base_of_v<Scene, T>
		void AddSceneConstructor(const std::string_view _strKey);

	private:
		std::unique_ptr<Scene>				m_activeScene;
		std::unordered_map<std::string_view, std::function<std::unique_ptr<Scene>()>> m_umapSceneConstructor;

		bool m_isFixedUpdating;
	};

	template<typename T> requires std::is_base_of_v<Scene, T>
	inline void SceneManager::AddSceneConstructor(const std::string_view _strKey)
	{
		m_umapSceneConstructor.insert(std::make_pair(_strKey,
			[]()->std::unique_ptr<Scene>
			{
				return std::unique_ptr<Scene>(static_cast<Scene*>(new T));
			}
		));
	}
}
