#include "Engine/EngineMain.h"
#include "Engine/resource.h"

#include "Engine/define_Macro.h"
#include "Engine/Util/AtExit.h"

#include "Engine/GameEngine.h"

namespace ehw
{
    std::vector<WindowMsgHandleFunc> EngineMain::m_commonMsgHandleFunctions;
    EngineMain* EngineMain::s_engineMainInst = nullptr;

    EngineMain::EngineMain(const tDesc_EngineMain& _Desc)
    : mInstance{}
    , m_hwnd{}
    , mHAccelTable{}
    {
        s_engineMainInst = this;

        mInstance = _Desc.Inst;

        AddCommonMsgHandleFunc(&Wm_Destroy);

        ASSERT(RegisterClientClass(_Desc), "창 생성 실패");
        ASSERT(InitInstance(_Desc), "창 생성 실패");

        ShowWindow(m_hwnd, SW_HIDE);
        UpdateWindow(m_hwnd);

        ehw::tGameEngineDesc engineDesc;
        engineDesc.Hwnd = m_hwnd;
        engineDesc.Height = _Desc.Height;
        engineDesc.Width = _Desc.Width;
        engineDesc.LeftWindowPos = _Desc.LeftPos;
        engineDesc.TopWindowPos = _Desc.TopPos;
        engineDesc.GPUDesc = _Desc.GPUDesc;
        engineDesc.EditorRunFunction = _Desc.EditorRunFunction;

        ehw::GameEngine::GetInst().init(engineDesc);

        for (size_t i = 0; i < _Desc.ExternalInitFuncs.size(); ++i)
        {
            if (_Desc.ExternalInitFuncs[i])
            {
                _Desc.ExternalInitFuncs[i]();
            }
        }

        mHAccelTable = LoadAccelerators(mInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    }

    EngineMain::~EngineMain()
    {
        AtExit::CallAtExit();

        s_engineMainInst = nullptr;

        

        mInstance = {};
        m_hwnd = {};

        m_commonMsgHandleFunctions.clear();
    }

    BOOL EngineMain::Run()
    {
        BOOL bReturn = TRUE;
        MSG msg{};
        // 기본 메시지 루프입니다:
        while (bReturn)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                //WMQUIT는 여기서 처리해줘야 함(DispatchMessage를 통해 WndProc로 전달되지 않음.)
                if (WM_QUIT == msg.message)
                {
                    bReturn = FALSE;
                    break;
                }

                //TranslateAccelerator: 메뉴 명령을 위한 메시지 처리
                if (!TranslateAccelerator(msg.hwnd, mHAccelTable, &msg))
                {
                    TranslateMessage(&msg);

                    //여기서 WndProc 함수를 호출한다.
                    DispatchMessage(&msg);
                }
            }
            else
            {
                bReturn = ehw::GameEngine::GetInst().Run();
            }
        }

        return bReturn;
    }

    void EngineMain::AddCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc)
    {
        if (_handleFunc)
        {
            m_commonMsgHandleFunctions.push_back(_handleFunc);
        }
    }

    void EngineMain::RemoveCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc)
    {
        for (auto iter = m_commonMsgHandleFunctions.begin(); m_commonMsgHandleFunctions.end() != iter; ++iter)
        {
            if (iter->target<void(*)>() == _handleFunc.target<void(*)>())
            {
                m_commonMsgHandleFunctions.erase(iter);
                break;
            }
        }
    }

    ATOM EngineMain::RegisterClientClass(const tDesc_EngineMain& _Desc)
    {
        WNDCLASSEX WinClass{};

        WinClass.cbSize = sizeof(WNDCLASSEX);

        WinClass.style = CS_HREDRAW | CS_VREDRAW;
        WinClass.lpfnWndProc = WndProc;
        WinClass.cbClsExtra = 0;
        WinClass.cbWndExtra = 0;
        WinClass.hInstance = mInstance;
        WinClass.hIcon = _Desc.WindowIcon;
        WinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        WinClass.lpszMenuName = NULL/*MAKEINTRESOURCEW(IDC_MY44ENGINE)*/;
        WinClass.lpszClassName = _Desc.ClassName;
        WinClass.hIconSm = _Desc.WindowIcon;

        return RegisterClassEx(&WinClass);
    }


    BOOL EngineMain::InitInstance(const tDesc_EngineMain& _Desc)
    {
        m_hwnd = CreateWindowW(_Desc.ClassName, _Desc.TitleName, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _Desc.Inst, nullptr);
        if (!m_hwnd)
        {
            return FALSE;
        }
        return TRUE;
    }

    LRESULT CALLBACK EngineMain::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        //참고
        //WM_CLOSE: 윈도우의 X버튼이나 Alt + F4 등의 버튼을 눌렀을 때 전달되는 메시지.
        //WM_DESTROY: WM_CLOSE 메시지가 DefWindowProc 함수에 들어갔을 때 전달되는 메시지.
        //WM_QUIT: PostQuitMessage 함수 호출 시 전달되는 메시지. 
        //  ㄴ참고: WM_QUIT 메시지는 WndProc 내부로 전달되지 않으므로 주의.
        for (size_t i = 0; i < m_commonMsgHandleFunctions.size(); ++i)
        {
            LRESULT result = m_commonMsgHandleFunctions[i](hWnd, message, wParam, lParam);
            if (result)
            {
                return result;
            }
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    LRESULT WINAPI EngineMain::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT ret = 0;
        if (message == WM_DESTROY) {
            GameEngine::GetInst().Destroy();
            PostQuitMessage(0);
            ret = 1;
        }
        return ret;
    }
}
