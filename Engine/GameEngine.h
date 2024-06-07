#pragma once
#include "Engine/Common.h"
#include "Engine/Util/StaticSingleton.h"

namespace ehw
{
	class GameEngine : public StaticSingleton<GameEngine>
	{
		friend class StaticSingleton<GameEngine>;
	public:
		static BOOL Init(const tGameEngineDesc& _AppDesc);

		static void Update();
		static void FinalUpdate();
		static void Render();

		//한 프레임 돌고 정리해야할 함수들 등록해놓으면 호출됨
		static void FrameEnd();

		// Running main engine loop
		static bool Run();

		static void Release();

		//사이즈를 입력하면 조절
		static void SetWindowPos(int _LeftWindowPos, int _TopWindowPos);
		static void SetWindowSize(int _Width, int _Height);
		static int2 GetWindowSize();

		static HWND GetHwnd() { return mHwnd; }

		static void Destroy();
		static void ShutDown() { mbInitialized = false; }

	private:
		static HWND mHwnd;
		static HDC  mHdc;

		static bool mbInitialized;

		static std::function<void()> m_editorRunFunction;
		
	public:
		GameEngine() = delete;
		~GameEngine() = delete;
	};
}
