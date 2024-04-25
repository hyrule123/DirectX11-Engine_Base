#include "Engine/Manager/TimeManager.h"

#include "Engine/Util/AtExit.h"
#include "Engine/GameEngine.h"

#include "Engine/GlobalVariable.h"

#include "Engine/define_Macro.h"

namespace ehw
{
    float                                   TimeManager::m_currentDeltaTime{};

    float	                                TimeManager::m_deltaTime{};
    float                                   TimeManager::m_delatTime_max_cap_per_frame{ g_default_deltatime_max_cap };

    std::chrono::steady_clock::time_point   TimeManager::m_prevTime{};
    std::chrono::steady_clock::time_point   TimeManager::m_currentTime{};

    float                                   TimeManager::m_fixedDeltaTime{ g_defaultFixedUpdateDeltaTime };
    uint				                    TimeManager::m_maxFixedUpdatesPerFrame{ g_maxFixedUpdatesPerFrame };

    float			                        TimeManager::m_accumulatedDeltaTime{};

    float			                        TimeManager::mOneSecond{};

    void TimeManager::Init()
    {
        AtExit::AddFunc(Release);

        m_prevTime = std::chrono::high_resolution_clock::now();

        SetMaxFixedUpdatesPerFrame(g_maxFixedUpdatesPerFrame);
        SetFixedDeltaTime(g_defaultFixedUpdateDeltaTime);
    }


    void TimeManager::Release()
    {
        m_currentDeltaTime = {};

        
        m_deltaTime = {};
        m_delatTime_max_cap_per_frame = {};

        m_prevTime = {};
        m_currentTime = {};
        //


        // FixedUpdate 관련
        m_fixedDeltaTime = {};
        m_maxFixedUpdatesPerFrame = {};

        m_accumulatedDeltaTime = {};
        //

        mOneSecond = {};
    }


    void TimeManager::Update()
    {
        //DeltaTime 계산
        {
            m_currentTime = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float> duration = m_currentTime - m_prevTime;
            m_deltaTime = duration.count();

            if (g_default_deltatime_max_cap < m_deltaTime)
            {
                m_deltaTime = g_default_deltatime_max_cap;
            }

            m_prevTime = m_currentTime;
        }

        m_currentDeltaTime = m_deltaTime;


        //static std::chrono::steady_clock::time_point end{};
        //static std::chrono::steady_clock::time_point start{};
        

        //if (m_fixedUpdateCount)
        //{
        //    end = std::chrono::high_resolution_clock::now();
        //    
        //    std::stringstream stream{};
        //    static std::chrono::duration<float> dur = end - start;


        //    stream << std::to_string(FixedDeltaTime()) << "\n";
        //    DEBUG_LOG_A(stream.str().c_str());
        //}
        //else
        //{
        //    start = std::chrono::high_resolution_clock::now();
        //}
    }

    uint TimeManager::GetFixedUpdateCount()
    {
        //FixedUpdate 횟수 계산
        uint count = 0u;

        if (m_fixedDeltaTime == 0.f)
        {
            count = 1u;
            m_accumulatedDeltaTime = 0.f;
        }
        else
        {
            m_accumulatedDeltaTime += m_deltaTime;
            count = (uint)(m_accumulatedDeltaTime / m_fixedDeltaTime);

            m_accumulatedDeltaTime -= ((float)count * m_fixedDeltaTime);
            if (m_accumulatedDeltaTime < 0.f)
            {
                m_accumulatedDeltaTime = 0.f;
            }

            count = m_maxFixedUpdatesPerFrame < count ? m_maxFixedUpdatesPerFrame : count;
        }

        return count;
    }



    void TimeManager::RenderFPS()
    {
#ifdef _DEBUG
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
#endif //_DEBUG


    }
    void TimeManager::SetFixedDeltaTime(float _fixedDeltaTime)
    {
        ASSERT(0.f < _fixedDeltaTime, "Fixed Delta Time은 음수가 될 수 없음.");

        m_fixedDeltaTime = _fixedDeltaTime;
    }
    void TimeManager::SetMaxFixedUpdatesPerFrame(uint _max)
    {
        ASSERT(0u < _max, "0으로는 설정할 수 없습니다.");

        m_maxFixedUpdatesPerFrame = _max;
    }
}
