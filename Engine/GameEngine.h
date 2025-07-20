#pragma once
#include <Engine/Common.h>
#include <Engine/Util/Singleton.h>

namespace core
{
	class GameEngine
	{
		DECLARE_SINGLETON(GameEngine);
	private:
		GameEngine();
		void init();

		~GameEngine();

	public:
		void init(const tGameEngineDesc& _AppDesc);

		void set_editor_func(const std::function<void()>& _editorFunc) {
			m_editor_run_function = _editorFunc;
		}

		// Running main engine loop
		BOOL run();

		//사이즈를 입력하면 조절
		void set_window_pos(int _LeftWindowPos, int _TopWindowPos);
		void set_window_size(int _Width, int _Height);
		int2 get_window_size();

		HWND get_HWND() { return m_hwnd; }

		void destroy();
		void shutdown() { m_bRunning = false; }

	private:
		void update();
		void final_update();
		void render();

		//한 프레임 돌고 정리해야할 함수들 등록해놓으면 호출됨
		void frame_end();

	private:
		HWND m_hwnd;
		HDC  m_hdc;

		std::function<void()> m_editor_run_function;

		BOOL m_bRunning : 1;
	};
}
