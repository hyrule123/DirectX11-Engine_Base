#include "Engine/MainWindow.h"
#include "Engine/resource.h"

#include "Engine/define_Macro.h"
#include "Engine/Util/AtExit.h"
         
#include "Engine/GameEngine.h"

HINSTANCE MainWindow::mInstance{};
HWND MainWindow::mHwnd{};
HACCEL MainWindow::mHAccelTable{};

std::unordered_map<UINT, std::vector<WindowMsgHandleFunc>, tHashFuncFast_UINT32> MainWindow::m_msgHandleFunctions{};

WindowMsgHandleFunc MainWindow::m_editorHandleFunction;
std::function<void()> MainWindow::m_editorRunFunction;

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


BOOL MainWindow::Init(const tDesc_GameMainWindow& _Desc)
{
    mInstance = _Desc.Inst;
    AddMsgHandleFunc(WM_COMMAND, Wm_Command);
    AddMsgHandleFunc(WM_PAINT, Wm_Paint);
    AddMsgHandleFunc(WM_DESTROY, Wm_Destroy);

    ASSERT(RegisterClientClass(_Desc), "창 생성 실패");
    ASSERT(InitInstance(_Desc), "창 생성 실패");

    ShowWindow(mHwnd, SW_HIDE);
    UpdateWindow(mHwnd);


    ehw::tDesc_Application AppDesc;
    AppDesc.Hwnd = mHwnd;
    AppDesc.Height = _Desc.Height;
    AppDesc.Width = _Desc.Width;
    AppDesc.LeftWindowPos = _Desc.LeftPos;
    AppDesc.TopWindowPos = _Desc.TopPos;
    AppDesc.GPUDesc = _Desc.GPUDesc;

    ehw::GameEngine::Init(AppDesc);
   
    for (size_t i = 0; i < _Desc.ExternalInitFuncs.size(); ++i)
    {
        if(_Desc.ExternalInitFuncs[i])
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

            if (m_editorRunFunction)
            {
                m_editorRunFunction();
            }

            ehw::GameEngine::Present();
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
    m_msgHandleFunctions.clear();

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
    if (m_editorHandleFunction)
    {
        LRESULT result = m_editorHandleFunction(hWnd, message, wParam, lParam);
        if (result)
        {
            return result;
        }
    }
 


    const auto& iter = m_msgHandleFunctions.find(message);
    if (iter != m_msgHandleFunctions.end())
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



LRESULT __stdcall MainWindow::Wm_Command(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

LRESULT __stdcall MainWindow::Wm_Paint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    EndPaint(hWnd, &ps);

    return 1;
}

LRESULT __stdcall MainWindow::Wm_Destroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
    return 1;
}


void MainWindow::RemoveMsgHandleFunc(UINT _Message, const WindowMsgHandleFunc& _HandleFunc)
{
    const auto& mapIter = m_msgHandleFunctions.find(_Message);
    if (mapIter != m_msgHandleFunctions.end())
    {
        const auto& vec = mapIter->second;
        for (auto vecIter = vec.begin(); vec.end() != vecIter; ++vecIter)
        {
            if (vecIter->target<void(*)>() == _HandleFunc.target<void(*)>())
            {
                mapIter->second.erase(vecIter);
                break;
            }
        }
    }
}

void MainWindow::RunEditor(const WindowMsgHandleFunc& _wndHandleFunc, const std::function<void()>& _runFunc)
{
    if (_wndHandleFunc && _runFunc)
    {
        m_editorHandleFunction = _wndHandleFunc;
        m_editorRunFunction = _runFunc;
    }
}

