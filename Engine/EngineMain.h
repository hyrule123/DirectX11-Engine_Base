#pragma once
#include "Engine/Common.h"
#include "Engine/Util/StaticSingleton.h"

//설명: 윈도우 인스턴스 초기화 및 GameEngine 루프를 실행하는곳
namespace ehw
{
	struct tDesc_EngineMain
	{
		HINSTANCE Inst;
		HICON	  WindowIcon;
		const TCHAR* TitleName;
		const TCHAR* ClassName;
		int LeftPos;
		int TopPos;
		int Width;
		int Height;

		std::vector<std::function<void()>> ExternalInitFuncs;

		ehw::tGPUManagerDesc GPUDesc;

		std::function<void()> EditorRunFunction;
	};

	class EngineMain
	{
	public:
		EngineMain(const tDesc_EngineMain& _Desc);
		~EngineMain();

		static inline EngineMain& get_inst() noexcept { return *s_engineMainInst; }

		BOOL Run();

		static void AddCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);
		static void RemoveCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);

	private:
		ATOM RegisterClientClass(const tDesc_EngineMain& _Desc);
		BOOL InitInstance(const tDesc_EngineMain& _Desc);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HINSTANCE mInstance;
		HWND m_hwnd;
		HACCEL mHAccelTable;

		//여러가지 메시지를 한번에 처리하는 함수는 여기 들어감(얘는 내부에서 뭘 처리할지 모르니 일단 호출)
		static std::vector<WindowMsgHandleFunc> m_commonMsgHandleFunctions;

		static EngineMain* s_engineMainInst;
	};
}




