#pragma once
#include "Engine/Common.h"
#include "Engine/Util/StaticSingleton.h"

namespace ehw
{
	class GameEngine : public StaticSingleton<GameEngine>
	{
		friend class StaticSingleton<GameEngine>;
	private:
		GameEngine();
		~GameEngine();
	public:
		BOOL init(const tGameEngineDesc& _AppDesc);

		void update();
		void final_update();
		void render();

		//한 프레임 돌고 정리해야할 함수들 등록해놓으면 호출됨
		void frame_end();

		// Running main engine loop
		bool Run();

		//사이즈를 입력하면 조절
		void SetWindowPos(int _LeftWindowPos, int _TopWindowPos);
		void SetWindowSize(int _Width, int _Height);
		int2 GetWindowSize();

		HWND GetHwnd() { return m_hwnd; }

		void Destroy();
		void ShutDown() { m_bRunning = false; }

	private:
		HWND m_hwnd;
		HDC  m_hdc;

		std::function<void()> m_editorRunFunction;

		bool m_bRunning;
	};
}
