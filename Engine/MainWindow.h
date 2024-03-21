#pragma once
#include "Engine/CommonStruct.h"

#include <vector>
#include <functional>


//설명: 윈도우 인스턴스 초기화 및 GameEngine 루프를 실행하는곳
namespace ehw
{
	struct tDesc_GameMainWindow
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

		ehw::tDesc_GPUMgr GPUDesc;

		std::function<void()> EditorRunFunction;
	};


	class MainWindow
	{
	public:
		static BOOL Run(const tDesc_GameMainWindow& _Desc);

		static void AddCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);
		static void RemoveCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc);

		static void AddSpecificMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _handleFunc);
		static void RemoveSpecificMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _handleFunc);

	private:
		static BOOL Init(const tDesc_GameMainWindow& _Desc);
		static BOOL Loop();
		static ATOM RegisterClientClass(const tDesc_GameMainWindow& _Desc);
		static BOOL InitInstance(const tDesc_GameMainWindow& _Desc);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static void Release();

		static LRESULT WINAPI Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		static HINSTANCE mInstance;
		static HWND mHwnd;
		static HACCEL mHAccelTable;

		//여러가지 메시지를 한번에 처리하는 함수는 여기 들어감(얘는 내부에서 뭘 처리할지 모르니 일단 호출)
		static std::vector<WindowMsgHandleFunc> m_commonMsgHandleFunctions;

		//특정 메시지 1개 처리하는 함수들은 여기 들어감
		static std::unordered_map<UINT, std::vector<WindowMsgHandleFunc>, tFastHasher<UINT>> m_specificMsgHandleFundtions;

	private:
		MainWindow() = delete;
		~MainWindow() = delete;
	};




}




