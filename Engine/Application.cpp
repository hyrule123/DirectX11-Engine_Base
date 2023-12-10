#include "PCH_Engine.h"
#include "Application.h"

#include "AtExit.h"

#include "ComponentInitializer.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CollisionManager.h"
#include "AudioManager.h"
#include "FontWrapper.h"
#include "PathManager.h"
#include "guiMgr.h"
#include "ThreadPoolManager.h"

#include "define_Global.h"

namespace ehw
{
	HWND			Application::mHwnd{};
	HDC				Application::mHdc{};
	bool			Application::mbInitialized{};

	BOOL Application::Init(const tDesc_Application& _AppDesc)
	{
		AtExit::AddFunc(Release);

		if (nullptr == _AppDesc.Hwnd)
		{
			return FALSE;
		}
		mHwnd = _AppDesc.Hwnd;

		SetWindowPos(_AppDesc.LeftWindowPos, _AppDesc.TopWindowPos);
		SetWindowSize(_AppDesc.Width, _AppDesc.Height);


		ThreadPoolManager::Init((size_t)std::thread::hardware_concurrency());



		PathManager::Init();

		//RenderMgr은 GPUMgr에서
		if (false == GPUManager::Init(_AppDesc.GPUDesc))
		{
			mHdc = GetDC(_AppDesc.Hwnd);
			ERROR_MESSAGE_W(L"Graphics Device 초기화 실패");
			return FALSE;
		}

		ResourceManager::Init();
		AudioManager::Init();
		FontWrapper::Init();


		ComponentInitializer::Init();
		
		TimeManager::Init();
		InputManager::Init();

		CollisionManager::Init();
		
		SceneManager::Init();

		mbInitialized = true;

		return TRUE;
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void Application::Update()
	{
		TimeManager::Update();
		InputManager::Update();
		CollisionManager::Update();
		SceneManager::Update();
	}

	// GPU update
	void Application::InternalUpdate()
	{
		CollisionManager::InternalUpdate();
		SceneManager::InternalUpdate();
	}

	void Application::Render()
	{
		TimeManager::Render(mHdc);

		//최종 렌더타겟 Clear
		GPUManager::ClearRenderTarget();

		RenderManager::Render();
	}

	void Application::FrameEnd()
	{		
		SceneManager::Destroy();

		SceneManager::FrameEnd();
	}

	// Running main engine loop
	bool Application::Run()
	{
		Update();
		InternalUpdate();
		Render();
		FrameEnd();

		gui::guiMgr::Run();
		
		return mbInitialized;
	}

	void Application::Present()
	{
		GPUManager::Present(true);
	}

	void Application::Release()
	{
		//EndFrame();
		ReleaseDC(mHwnd, mHdc);
		mbInitialized = false;
	}

	void Application::SetWindowPos(int _LeftWindowPos, int _TopWindowPos)
	{
		//가로세로 길이는 유지하고 위치만 변경
		UINT flag = SWP_NOSIZE | SWP_NOZORDER;
		::SetWindowPos(mHwnd, nullptr, _LeftWindowPos, _TopWindowPos, 0, 0, flag);
	}
	void Application::SetWindowSize(int _width, int _height)
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcWindow, rcClient;
		GetWindowRect(mHwnd, &rcWindow);
		GetClientRect(mHwnd, &rcClient);

		// calculate size of non-client area
		int xExtra = rcWindow.right - rcWindow.left - rcClient.right;
		int yExtra = rcWindow.bottom - rcWindow.top - rcClient.bottom;

		// now resize based on desired client size
		::SetWindowPos(mHwnd, 0, 0u, 0u, _width + xExtra, _height + yExtra, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		::ShowWindow(mHwnd, true);
		::UpdateWindow(mHwnd);
	}

	int2 Application::GetWindowSize()
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcClient{};
		GetClientRect(mHwnd, &rcClient);

		return int2{ rcClient.right, rcClient.bottom };
	}

}
