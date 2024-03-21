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
        AddSpecificMsgHandleFunc(WM_COMMAND, Wm_Command);
        AddSpecificMsgHandleFunc(WM_PAINT, Wm_Paint);
        AddSpecificMsgHandleFunc(WM_DESTROY, Wm_Destroy);

        ASSERT(RegisterClientClass(_Desc), "창 생성 실패");
        ASSERT(InitInstance(_Desc), "창 생성 실패");

        ShowWindow(mHwnd, SW_HIDE);
        UpdateWindow(mHwnd);

        ehw::tDesc_GameEngine engineDesc;
        engineDesc.Hwnd = mHwnd;
        engineDesc.Height = _Desc.Height;
        engineDesc.Width = _Desc.Width;
        engineDesc.LeftWindowPos = _Desc.LeftPos;
        engineDesc.TopWindowPos = _Desc.TopPos;
        engineDesc.GPUDesc = _Desc.GPUDesc;
        engineDesc.EditorRunFunction = _Desc.EditorRunFunction;

        ehw::GameEngine::Init(engineDesc);

        for (size_t i = 0; i < _Desc.ExternalInitFuncs.size(); ++i)
        {
            if (_Desc.ExternalInitFuncs[i])
                _Desc.ExternalInitFuncs[i]();
        }

        mHAccelTable = LoadAccelerators(mInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
        return true;
    }

    BOOL MainWindow::Loop()
    {
        BOOL bReturn = TRUE;
        MSG msg;
        // 기본 메시지 루프입니다:
        while (bReturn)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                if (WM_QUIT == msg.message)
                {
                    bReturn = FALSE;
                    break;
                }


                if (!TranslateAccelerator(msg.hwnd, mHAccelTable, &msg))
                {
                    TranslateMessage(&msg);
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

        m_editorHandleFunction = nullptr;
        m_editorRunFunction = nullptr;
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



    LRESULT WINAPI MainWindow::Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        //switch (wmId)
        //{
        //default:
        //    return DefWindowProc(hWnd, message, wParam, lParam);
        //}

        return 1;
    }

    LRESULT WINAPI MainWindow::Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);

        return 1;
    }

    LRESULT WINAPI MainWindow::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PostQuitMessage(0);
        return 1;
    }




    void MainWindow::RunEditor(const WindowMsgHandleFunc& _wndHandleFunc, const std::function<void()>& _runFunc)
    {
        if (_wndHandleFunc && _runFunc)
        {
            m_editorHandleFunction = _wndHandleFunc;
            m_editorRunFunction = _runFunc;
        }
    }


}
