#include "Engine/GameEngine.h"

#include "Engine/Util/AtExit.h"

#include "Engine/Game/Collision/PhysXInstance.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/TimeManager.h"
#include "Engine/Manager/InputManager.h"
#include "Engine/Manager/SceneManager.h"
#include "Engine/Manager/ResourceManagers.h"
#include "Engine/Manager/AudioManager.h"
#include "Engine/Manager/FontWrapper.h"
#include "Engine/Manager/PathManager.h"
#include "Engine/Manager/ThreadPoolManager.h"


#include "Engine/Util/StaticSingleton.h"

#include "Engine/EngineMain.h"

#include "Engine/GlobalVariable.h"

#include "Engine/Game/Scene.h"

namespace core
{
	GameEngine::GameEngine()
		: m_hwnd{ nullptr }
		, m_hdc{ nullptr }
		, m_editorRunFunction { nullptr }
		, m_bRunning { false }
	{
	}

	GameEngine::~GameEngine()
	{
		::ReleaseDC(m_hwnd, m_hdc);

		m_hwnd = {};
		m_hdc = {};
		m_bRunning = false;
		m_editorRunFunction = nullptr;

		//InstanceManager은 수동 제거(static 초기화 타임에 생성되어 static 컨테이너를 사용하는 AtExit를 사용할 수가 없음..)
		EntityFactory::Destroy();
	}

	BOOL GameEngine::init(const tGameEngineDesc& _desc)
	{
		if (nullptr == _desc.Hwnd)
		{
			return FALSE;
		}
		m_hwnd = _desc.Hwnd;
		m_hdc = GetDC(_desc.Hwnd);

		SetWindowPos(_desc.LeftWindowPos, _desc.TopWindowPos);
		SetWindowSize(_desc.Width, _desc.Height);

		//(size_t)std::thread::hardware_concurrency()를 사용하면 CPU 스레드 수를 받아올 수 있다.
		ThreadPoolManager::get_inst().init((size_t)4);

		//PathManager 인스턴스 생성
		PathManager::get_inst();
		
		//RenderManager 인스턴스 생성(Device, Context 생성)
		RenderManager::get_inst();

		//ResourceManager들 인스턴스 생성
		ResourceManagers::get_inst().init_resource_managers();

		//래스터라이저, 블렌드, 깊이버퍼 등 각종 render state 로드
		RenderManager::get_inst().load_render_states();

		//기본 리소스(메쉬, 쉐이더, 재질, 텍스처) 로드
		ResourceManagers::get_inst().load_default_resources();

		//RenderManager 스왑체인 및 뷰포트 생성
		RenderManager::get_inst().init(_desc.GPUDesc);
		
		AudioManager::get_inst();
		FontWrapper::get_inst();
		
		TimeManager::get_inst();

		InputManager::get_inst();
		
		PhysXInstance::get_inst();
		SceneManager::get_inst();

		m_editorRunFunction = _desc.EditorRunFunction;

		m_bRunning = true;

		return TRUE;
	}

	// 게임 로직 캐릭터 이동 등등 
	// CPU UPDATE
	void GameEngine::update()
	{
		TimeManager::get_inst().update();
		InputManager::get_inst().update();
		SceneManager::get_inst().fixed_update();
		SceneManager::get_inst().update();
	}

	// GPU에 보내기 위한 최종 정보 정리
	void GameEngine::final_update()
	{
		SceneManager::get_inst().final_update();
	}

	void GameEngine::render()
	{
		//최종 렌더타겟 Clear
		RenderManager::get_inst().clear_rendertarget();

		RenderManager::get_inst().render();

		if (m_editorRunFunction)
		{
			m_editorRunFunction();
		}

		TimeManager::get_inst().RenderFPS();

		RenderManager::get_inst().present(true);
	}

	void GameEngine::frame_end()
	{	
		RenderManager::get_inst().FrameEnd();
		SceneManager::get_inst().frame_end();
	}

	// Running main engine loop
	bool GameEngine::Run()
	{
		update();
		final_update();
		render();

		frame_end();
		
		return m_bRunning;
	}

	void GameEngine::SetWindowPos(int _LeftWindowPos, int _TopWindowPos)
	{
		//가로세로 길이는 유지하고 위치만 변경
		UINT flag = SWP_NOSIZE | SWP_NOZORDER;
		::SetWindowPos(m_hwnd, nullptr, _LeftWindowPos, _TopWindowPos, 0, 0, flag);
	}
	void GameEngine::SetWindowSize(int _width, int _height)
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcWindow, rcClient;
		::GetWindowRect(m_hwnd, &rcWindow);
		::GetClientRect(m_hwnd, &rcClient);

		// calculate size of non-client area
		int xExtra = rcWindow.right - rcWindow.left - rcClient.right;
		int yExtra = rcWindow.bottom - rcWindow.top - rcClient.bottom;

		// now resize based on desired client size
		::SetWindowPos(m_hwnd, 0, 0u, 0u, _width + xExtra, _height + yExtra, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		::ShowWindow(m_hwnd, true);
		::UpdateWindow(m_hwnd);
	}
	

	int2 GameEngine::GetWindowSize()
	{
		//클라이언트 영역과 윈도우 영역의 차이를 구해서 정확한 창 크기를 설정(해상도가 조금이라도 차이나면 문제 발생함)
		RECT rcClient{};
		::GetClientRect(m_hwnd, &rcClient);

		return int2{ rcClient.right, rcClient.bottom };
	}

	void GameEngine::Destroy()
	{
		//Destroy() 호출 후
		SceneManager::get_inst().Destroy();
		
		Run();
		frame_end();

		//한 프레임 돌려주고(충돌체 해제 등의 작업 진행) 끝낸다
		
		m_bRunning = false;
	}

}
