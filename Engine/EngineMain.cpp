#include "Engine/EngineMain.h"
#include "Engine/resource.h"

#include "Engine/define_Macro.h"
#include "Engine/Util/AtExit.h"

#include "Engine/GameEngine.h"

namespace core
{
    EngineMain::EngineMain(const tDesc_EngineMain& _setting)
    {
        m_hinstance = _setting.hInstance;

        add_common_msg_handle_func(&wm_destroy);

        ASSERT(register_client_class(_setting), "창 생성 실패");
        ASSERT(init_instance(_setting), "창 생성 실패");

        //if문 체크 말고 다른방식으로 하면 자꾸 경고뜸
        if (m_hwnd) {
            ShowWindow(m_hwnd, SW_HIDE);
        }
        else {
            ASSERT(false, "HWND가 생성되지 않음.");
        }

        UpdateWindow(m_hwnd);

        core::tGameEngineDesc engineDesc;
        engineDesc.Hwnd = m_hwnd;
        engineDesc.Height = _setting.height;
        engineDesc.Width = _setting.width;
        engineDesc.LeftWindowPos = _setting.leftPos;
        engineDesc.TopWindowPos = _setting.topPos;
        engineDesc.GPUDesc = _setting.GPUDesc;

        GameEngine::get_inst().init(engineDesc);

        if (_setting.entryFunction)
        {
            _setting.entryFunction();
        }

        m_hacceltable = LoadAccelerators(m_hinstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
    }

    EngineMain::~EngineMain()
    {
        AtExit::on_exit();

        m_hinstance = {};
        m_hwnd = {};
        m_commonMsgHandleFunctions.clear();
    }

    BOOL EngineMain::run()
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
                if (!TranslateAccelerator(msg.hwnd, m_hacceltable, &msg))
                {
                    TranslateMessage(&msg);

                    //여기서 WndProc 함수를 호출한다.
                    DispatchMessage(&msg);
                }
            }
            else
            {
                bReturn = core::GameEngine::get_inst().run();
            }
        }

        return bReturn;
    }

    void EngineMain::add_common_msg_handle_func(const WindowMsgHandleFunc& _handleFunc)
    {
        if (_handleFunc)
        {
            m_commonMsgHandleFunctions.push_back(_handleFunc);
        }
    }

    void EngineMain::remove_common_msg_handle_func(const WindowMsgHandleFunc& _handleFunc)
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

    ATOM EngineMain::register_client_class(const tDesc_EngineMain& _Desc)
    {
        WNDCLASSEX WinClass{};

        WinClass.cbSize = sizeof(WNDCLASSEX);

        WinClass.style = CS_HREDRAW | CS_VREDRAW;
        WinClass.lpfnWndProc = wnd_proc;
        WinClass.cbClsExtra = 0;
        WinClass.cbWndExtra = 0;
        WinClass.hInstance = m_hinstance;
        WinClass.hIcon = _Desc.windowIcon;
        WinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        WinClass.lpszMenuName = NULL/*MAKEINTRESOURCEW(IDC_MY44ENGINE)*/;
        WinClass.lpszClassName = _Desc.className;
        WinClass.hIconSm = _Desc.windowIcon;

        return RegisterClassEx(&WinClass);
    }


    BOOL EngineMain::init_instance(const tDesc_EngineMain& _Desc)
    {
        m_hwnd = CreateWindowW(_Desc.className, _Desc.titleName, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _Desc.hInstance, nullptr);
        if (!m_hwnd)
        {
            return FALSE;
        }
        return TRUE;
    }

    LRESULT CALLBACK EngineMain::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

    LRESULT WINAPI EngineMain::wm_destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT ret = 0;
        if (message == WM_DESTROY) {
            GameEngine::get_inst().destroy();
            PostQuitMessage(0);
            ret = 1;
        }
        return ret;
    }
}
