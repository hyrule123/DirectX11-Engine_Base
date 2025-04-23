#pragma once
#include "Engine/Common.h"

//설명: 윈도우 인스턴스 초기화 및 GameEngine 루프를 실행하는곳
namespace core
{
	struct tDesc_EngineMain
	{
		HINSTANCE hInstance;
		HICON	  windowIcon;
		const TCHAR* titleName;
		const TCHAR* className;
		int leftPos;
		int topPos;
		int width;
		int height;

		std::function<void()> entryFunction;

		core::tGPUManagerDesc GPUDesc;
	};

	class EngineMain
	{
	public:
		EngineMain(const tDesc_EngineMain& _setting);
		~EngineMain();

		BOOL Run();

		static void AddCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);
		static void RemoveCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);

	private:
		ATOM RegisterClientClass(const tDesc_EngineMain& _Desc);
		BOOL InitInstance(const tDesc_EngineMain& _Desc);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HINSTANCE m_hinstance;
		HWND m_hwnd;
		HACCEL m_hacceltable;

		//여러가지 메시지를 한번에 처리하는 함수는 여기 들어감(얘는 내부에서 뭘 처리할지 모르니 일단 호출)
		static inline std::vector<WindowMsgHandleFunc> m_commonMsgHandleFunctions;
	};
}




