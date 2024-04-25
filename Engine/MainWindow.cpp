#include "Engine/MainWindow.h"
#include "Engine/resource.h"

#include "Engine/define_Macro.h"
#include "Engine/Util/AtExit.h"

#include "Engine/GameEngine.h"

namespace ehw
{
    HINSTANCE MainWindow::mInstance{};
    HWND MainWindow::mHwnd{};
    HACCEL MainWindow::mHAccelTable{};

    std::vector<WindowMsgHandleFunc> MainWindow::m_commonMsgHandleFunctions{};
    std::unordered_map<UINT, std::vector<WindowMsgHandleFunc>, tFastHasher<UINT>> MainWindow::m_specificMsgHandleFundtions{};


    BOOL MainWindow::Run(const tDesc_GameMainWindow& _Desc)
    {

        BOOL bResult = FALSE;
        bResult = Init(_Desc);

        while (bResult)
        {
            bResult = Loop();
        }

        Release();
        return bResult;
    }

    void MainWindow::AddCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc)
    {
        if (nullptr == _handleFunc)
        {
            return;
        }

        m_commonMsgHandleFunctions.push_back(_handleFunc);
    }

    void MainWindow::RemoveCommonMsgHandleFunc(const WindowMsgHandleFunc& _handleFunc)
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

    void MainWindow::AddSpecificMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _handleFunc)
    {
        if (nullptr == _handleFunc)
            return;

        m_specificMsgHandleFundtions[_Message].push_back(_handleFunc);
    }

    void MainWindow::RemoveSpecificMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _handleFunc)
    {
        const auto& mapIter = m_specificMsgHandleFundtions.find(_Message);
        if (mapIter != m_specificMsgHandleFundtions.end())
        {
            const auto& vec = mapIter->second;
            for (auto vecIter = vec.begin(); vec.end() != vecIter; ++vecIter)
            {
                if (vecIter->target<void(*)>() == _handleFunc.target<void(*)>())
                {
                    mapIter->second.erase(vecIter);
                    break;
                }
            }
        }
    }

    BOOL MainWindow::Init(const tDesc_GameMainWindow& _Desc)
    {
        mInstance = _Desc.Inst;
        AddSpecificMsgHandleFunc(WM_DESTROY, Wm_Destroy);

        ASSERT(RegisterClientClass(_Desc), "창 생성 실패");
        ASSERT(InitInstance(_Desc), "창 생성 실패");

        ShowWindow(mHwnd, SW_HIDE);
        UpdateWindow(mHwnd);

        ehw::tGameEngineDesc engineDesc;
        engineDesc.Hwnd = mHwnd;
        engineDesc.Height = _Desc.Height;
        engineDesc.Width = _Desc.Width;
        engineDesc.LeftWindowPos = _Desc.LeftPos;
        engineDesc.TopWindowPos = _Desc.TopPos;
        engineDesc.GPUDesc = _Desc.GPUDesc;
        engineDesc.EditorRunFunction = _Desc.EditorRunFunction;
        engineDesc.FPS_Limit = _Desc.FPS_Limit;

        ehw::GameEngine::Init(engineDesc);

        for (size_t i = 0; i < _Desc.ExternalInitFuncs.size(); ++i)
        {
            if (_Desc.ExternalInitFuncs[i])
            {
                _Desc.ExternalInitFuncs[i]();
            }
        }



        mHAccelTable = LoadAccelerators(mInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
        return true;
    }

    BOOL MainWindow::Loop()
    {
        BOOL bReturn = TRUE;
        MSG msg{};
        // 기본 메시지 루프입니다:
        while (bReturn)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                //WMQUIT는 여기서 처리해줘야 함(DispatchMessage를 통해 WndProc로 전달되지 않음.
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
                bReturn = ehw::GameEngine::Run();
            }
        }

        return bReturn;
    }

    ATOM MainWindow::RegisterClientClass(const tDesc_GameMainWindow& _Desc)
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

    void MainWindow::Release()
    {
        AtExit::CallAtExit();

        mInstance = {};
        mHwnd = {};

        m_commonMsgHandleFunctions.clear();
        m_specificMsgHandleFundtions.clear();
    }




    BOOL MainWindow::InitInstance(const tDesc_GameMainWindow& _Desc)
    {
        mHwnd = CreateWindowW(_Desc.ClassName, _Desc.TitleName, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _Desc.Inst, nullptr);
        if (!mHwnd)
        {
            return FALSE;
        }
        return TRUE;
    }

    LRESULT CALLBACK MainWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

        const auto& iter = m_specificMsgHandleFundtions.find(message);
        if (iter != m_specificMsgHandleFundtions.end())
        {
            for (size_t i = 0; i < iter->second.size(); ++i)
            {
                if (iter->second[i])
                {
                    LRESULT result = iter->second[i](hWnd, message, wParam, lParam);
                    if (result)
                        return result;
                }
            }
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    LRESULT WINAPI MainWindow::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        GameEngine::Destroy();
        PostQuitMessage(0);
        return 1;
    }
}
