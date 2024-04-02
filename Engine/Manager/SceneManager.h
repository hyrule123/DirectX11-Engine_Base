#pragma once
#include "Engine/Game/iScene.h"

namespace ehw
{
	class SceneManager
	{
	public:
		static void Init();
		static void Update();
		static void CollisionUpdate();
		static void FinalUpdate();
		static void Render();
		static void FrameEnd();
		static void Release();

		static void Destroy();

		static void		LoadScene(const std::string_view _strKey);
		static iScene*	GetActiveScene() { return m_activeScene.get(); }

		template <typename T>
		static void AddSceneConstructor(const std::string_view _strKey);

	private:
		static std::unique_ptr<iScene>				m_activeScene;
		static std::unordered_map<std::string_view, std::function<iScene* ()>> m_umapSceneConstructor;
	};

	template<typename T>
	inline void SceneManager::AddSceneConstructor(const std::string_view _strKey)
	{
		static_assert(std::is_base_of_v<iScene, T>);
		m_umapSceneConstructor.insert(std::make_pair(_strKey,
			[]()->T*
			{
				return new T;
			}
		));
	}
}
