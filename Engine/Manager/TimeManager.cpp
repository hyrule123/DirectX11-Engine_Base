#include "Engine/Manager/TimeManager.h"

#include "Engine/Util/AtExit.h"
#include "Engine/GameEngine.h"

#include "Engine/CommonGlobalVar.h"

namespace ehw
{
    float	            TimeManager::m_deltaTime{};

    std::chrono::steady_clock::time_point TimeManager::m_prevTime{};
    std::chrono::steady_clock::time_point TimeManager::m_currentTime{};

    float			    TimeManager::mOneSecond{};

    void TimeManager::Init()
    {
        auto start = std::chrono::high_resolution_clock::now();

        AtExit::AddFunc(Release);

        m_prevTime = std::chrono::high_resolution_clock::now();
    }


    void TimeManager::Release()
    {
        m_deltaTime = {};

        m_prevTime = {};
        m_currentTime = {};

        mOneSecond = {};
    }


    void TimeManager::Update()
    {
        m_currentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration = m_currentTime - m_prevTime;
        m_deltaTime = duration.count();

        if (g_deltaTimeMaxCap < m_deltaTime)
        {
            m_deltaTime = g_deltaTimeMaxCap;
        }

        m_prevTime = m_currentTime;
    }

    void TimeManager::Render(HDC _hdc)
    {
        static int iCount = 0;
        ++iCount;


        // 1 초에 한번
        mOneSecond += m_deltaTime;
        if (1.0f < mOneSecond)
        {
            HWND hWnd = GameEngine::GetHwnd();

            wchar_t szFloat[50] = {};
            float FPS = 1.f / m_deltaTime;
            swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            mOneSecond = 0.f;
            iCount = 0;
        }
    }
}
