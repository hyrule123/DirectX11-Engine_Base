#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Util/SimpleMath.h"
#include "define_Struct.h"

namespace ehw
{
	class Application
	{
	public:
		//struct tDesc_Application
		//{
		//	HWND Hwnd;
		//	int  LeftWindowPos;
		//	int	 TopWindowPos;
		//	int  Width;
		//	int  Height;
		//	tDesc_GPUMgr GPUDesc;
		//};
		static BOOL Init(const tDesc_Application& _AppDesc);

		static void Update();
		static void InternalUpdate();
		static void Render();

		//한 프레임 돌고 정리해야할 함수들 등록해놓으면 호출됨
		static void FrameEnd();

		// Running main engine loop
		static bool Run();

		static void Present();
		static void Release();

		//사이즈를 입력하면 조절
		static void SetWindowPos(int _LeftWindowPos, int _TopWindowPos);
		static void SetWindowSize(int _Width, int _Height);
		static int2 GetWindowSize();

		static HWND GetHwnd() { return mHwnd; }

		static void ShutDown() { mbInitialized = false; }

	private:
		static HWND mHwnd;
		static HDC  mHdc;
		static bool mbInitialized;

		
	public:
		Application() = delete;
		~Application() = delete;
	};
}
