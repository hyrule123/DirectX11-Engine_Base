#pragma once
#include "iScene.h"

namespace ehw
{
	class SceneMgr
	{
	public:
		static void Init();
		static void Update();
		static void InternalUpdate();
		static void Render();
		static void Destroy();
		static void FrameEnd();
		static void Release();

		static void		LoadScene(const std::string_view _strKey);
		static iScene*	GetActiveScene() { return mActiveScene.get(); }

		template <typename T>
		static void AddSceneConstructor(const std::string_view _strKey);
		

	private:
		static std::unique_ptr<iScene>				mActiveScene;


		static std::unordered_map<std::string_view, std::function<iScene* ()>> mUmapSceneConstructor;
	};

	template<typename T>
	inline void SceneMgr::AddSceneConstructor(const std::string_view _strKey)
	{
		static_assert(std::is_base_of_v<iScene, T>);
		mUmapSceneConstructor.insert(std::make_pair(_strKey,
			[]()->T*
			{
				return new T;
			}
		));
	}
}
