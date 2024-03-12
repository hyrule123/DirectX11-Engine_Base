#pragma once
#include "Engine/CommonStruct.h"

#include <vector>
#include <functional>


//설명: 윈도우 인스턴스 초기화 및 GameEngine 루프를 실행하는곳

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

	WindowMsgHandleFunc EditorHandleFunction;
	std::function<void()> EditorRunFunction;
};

struct tHashFuncFast_UINT32
{
	UINT64 operator()(const UINT32& _ukey) const
	{
		return static_cast<UINT64>(_ukey);
	}
};


class MainWindow
{
public:
	static BOOL Run(const tDesc_GameMainWindow& _Desc);

	static void AddMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _HandleFunc);
	static void RemoveMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _HandleFunc);

	static void RunEditor(const WindowMsgHandleFunc& _wndHandleFunc, const std::function<void()>& _runFunc);
	
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


	//특정 메시지를 처리하는 함수들은 여기
	static std::unordered_map<UINT, std::vector<WindowMsgHandleFunc>, tHashFuncFast_UINT32> m_msgHandleFunctions;

	static WindowMsgHandleFunc m_editorHandleFunction;
	static std::function<void()> m_editorRunFunction;

private:
	MainWindow() = delete;
	~MainWindow() = delete;
};



inline void MainWindow::AddMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _HandleFunc)
{
	if (nullptr == _HandleFunc)
		return;

	m_msgHandleFunctions[_Message].push_back(_HandleFunc);
}


