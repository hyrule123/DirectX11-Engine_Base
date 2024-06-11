#pragma once
#include "Engine/Util/StaticSingleton.h"

#include "Engine/Scene/iScene.h"

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
		void Render();
		void FrameEnd();
		void Release();

		void Destroy();

		void		LoadScene(const std::string_view _strKey);
		iScene*	GetActiveScene() { return m_activeScene.get(); }

		template <typename T> requires std::is_base_of_v<iScene, T>
		void AddSceneConstructor(const std::string_view _strKey);

	private:
		std::unique_ptr<iScene>				m_activeScene;
		std::unordered_map<std::string_view, std::function<std::unique_ptr<iScene>()>> m_umapSceneConstructor;

		bool m_isFixedUpdating;
	};

	template<typename T> requires std::is_base_of_v<iScene, T>
	inline void SceneManager::AddSceneConstructor(const std::string_view _strKey)
	{
		m_umapSceneConstructor.insert(std::make_pair(_strKey,
			[]()->std::unique_ptr<iScene>
			{
				return std::unique_ptr<iScene>(static_cast<iScene*>(new T));
			}
		));
	}
}
