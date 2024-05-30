#include "Engine/GameEngine.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Scene/Collision/PhysXInstance.h"
#include "Engine/Scene/Component/ComponentInitializer.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/InputManager.h"
#include "Engine/Manager/SceneManager.h"
#include "Engine/Manager/ResourceManagers.h"
#include "Engine/Manager/AudioManager.h"
#include "Engine/Manager/FontWrapper.h"
#include "Engine/Manager/PathManager.h"
#include "Engine/Manager/ThreadPoolManager.h"
#include "Engine/Manager/GPUManager.h"



#include "Engine/MainWindow.h"

#include "Engine/GlobalVariable.h"

namespace ehw
{
	HWND			GameEngine::mHwnd{};
	HDC				GameEngine::mHdc{};
	bool			GameEngine::mbInitialized = false;
	std::function<void()> GameEngine::m_editorRunFunction = nullptr;

	BOOL GameEngine::Init(const tGameEngineDesc& _desc)
	{
		AtExit::AddFunc(Release);

		if (nullptr == _desc.Hwnd)
		{
			return FALSE;
		}
		mHwnd = _desc.Hwnd;

		SetWindowPos(_desc.LeftWindowPos, _desc.TopWindowPos);
		SetWindowSize(_desc.Width, _desc.Height);

		ThreadPoolManager::Init((size_t)std::thread::hardware_concurrency());

		PathManager::Init();

		//RenderMgr은 GPUMgr에서
		if (false == GPUManager::Init(_desc.GPUDesc))
		{
			mHdc = GetDC(_desc.Hwnd);
			ERROR_MESSAGE("Graphics Device 초기화 실패");
			return FALSE;
		}

		ResourceManagers::Init();

		RenderManager::Init();
		AudioManager::Init();
		FontWrapper::Init();

		ComponentInitializer::Init();
		
		TimeManager::Init();

		InputManager::Init();
		
		PhysXInstance::Init();
		SceneManager::Init();

		m_editorRunFunction = _desc.EditorRunFunction;

		mbInitialized = true;

		return TRUE;
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void GameEngine::Update()
	{
		TimeManager::Update();
		InputManager::Update();
		SceneManager::FixedUpdate();
		SceneManager::Update();
	}

	// GPU에 보내기 위한 최종 정보 정리
	void GameEngine::FinalUpdate()
	{
		SceneManager::FinalUpdate();
	}

	void GameEngine::Render()
	{
		//최종 렌더타겟 Clear
		GPUManager::ClearRenderTarget();

		RenderManager::Render();

		if (m_editorRunFunction)
		{
			m_editorRunFunction();
		}

		TimeManager::RenderFPS();

		GPUManager::Present(true);
	}

	void GameEngine::FrameEnd()
	{	
		RenderManager::FrameEnd();
		SceneManager::FrameEnd();
	}

	// Running main engine loop
	bool GameEngine::Run()
	{
		Update();
		FinalUpdate();
		Render();

		FrameEnd();
		
		return mbInitialized;
	}

	void GameEngine::Release()
	{
		::ReleaseDC(mHwnd, mHdc);

		mHwnd = {};
		mHdc = {};
		mbInitialized = false;
		m_editorRunFunction = nullptr;
	}

	void GameEngine::SetWindowPos(int _LeftWindowPos, int _TopWindowPos)
	{
		//가로세로 길이는 유지하고 위치만 변경
		UINT flag = SWP_NOSIZE | SWP_NOZORDER;
		::SetWindowPos(mHwnd, nullptr, _LeftWindowPos, _TopWindowPos, 0, 0, flag);
	}
	void GameEngine::SetWindowSize(int _width, int _height)
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcWindow, rcClient;
		::GetWindowRect(mHwnd, &rcWindow);
		::GetClientRect(mHwnd, &rcClient);

		// calculate size of non-client area
		int xExtra = rcWindow.right - rcWindow.left - rcClient.right;
		int yExtra = rcWindow.bottom - rcWindow.top - rcClient.bottom;

		// now resize based on desired client size
		::SetWindowPos(mHwnd, 0, 0u, 0u, _width + xExtra, _height + yExtra, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		::ShowWindow(mHwnd, true);
		::UpdateWindow(mHwnd);
	}
	

	int2 GameEngine::GetWindowSize()
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcClient{};
		::GetClientRect(mHwnd, &rcClient);

		return int2{ rcClient.right, rcClient.bottom };
	}

	void GameEngine::Destroy()
	{
		//Destroy() 호출 후
		SceneManager::Destroy();
		
		Run();
		FrameEnd();

		//한 프레임 돌려주고(충돌체 해제 등의 작업 진행) 끝낸다
		
		mbInitialized = false;
	}

}
