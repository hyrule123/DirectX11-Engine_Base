#include "Engine/Manager/TimeManager.h"

#include "Engine/Util/AtExit.h"
#include "Engine/GameEngine.h"

#include "Engine/GlobalVariable.h"

#include "Engine/define_Macro.h"

namespace ehw
{
    float	            TimeManager::m_deltaTime{};
    float               TimeManager::m_delatTime_max_cap_per_frame{ g_default_deltatime_max_cap };

    float			    TimeManager::m_accumulatedDeltaTime{};
    eFrameTimeStep	    TimeManager::m_minFixedUpdateTimeStep{ g_default_FixedUpdate_TimeStep };
    eFrameTimeStep	    TimeManager::m_currentTimeStep{ g_default_FixedUpdate_TimeStep };
    int				TimeManager::m_fixedUpdateCount{};
    int				TimeManager::m_maxFixedUpdatesPerFrame{ g_default_FixedUpdate_max_count_per_frame } ;

    std::chrono::steady_clock::time_point TimeManager::m_prevTime{};
    std::chrono::steady_clock::time_point TimeManager::m_currentTime{};

    float			    TimeManager::mOneSecond{};

    void TimeManager::Init()
    {
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

        //FixedUpdate 횟수 계산
        {
            m_accumulatedDeltaTime += m_deltaTime;
            
            m_fixedUpdateCount = (int)(m_accumulatedDeltaTime / GetFrameTimeStep(m_currentTimeStep));
            if (m_maxFixedUpdatesPerFrame < m_fixedUpdateCount)
            {
                m_fixedUpdateCount = m_maxFixedUpdatesPerFrame;
            }
            
            m_accumulatedDeltaTime -= GetFrameTimeStep(m_currentTimeStep) * (float)m_fixedUpdateCount;
            if (m_accumulatedDeltaTime < 0.f)
            {
                m_accumulatedDeltaTime = 0.f;
            }
        }


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
    void TimeManager::AdjustFixedUpdateTimeStep(const float _fixedUpdateDuration)
    {
#ifdef _DEBUG
        if (1.f < _fixedUpdateDuration)
        {
            return;
        }
#endif

        int minTimeStep = (int)m_minFixedUpdateTimeStep;
        int currentTimeStep = (int)m_currentTimeStep;

        //최소 TimeStep부터 최대 TimeStep까지 순회 돌아주면서 현재 FixedUpdate에 걸리는 시간에 맞게 조정한다.
        for (minTimeStep; minTimeStep < (int)eFrameTimeStep::END; ++minTimeStep)
        {
            if ((minTimeStep + 1) == (int)eFrameTimeStep::END)
            {
                break;
            }

            else if (_fixedUpdateDuration <= GetFrameTimeStep(minTimeStep))
            {
                break;
            }

            else if (
                GetFrameTimeStep(minTimeStep) <= _fixedUpdateDuration
                && _fixedUpdateDuration <= GetFrameTimeStep(minTimeStep + 1))
            {
                break;
            }
        }

        m_currentTimeStep = (eFrameTimeStep)minTimeStep;
    }
}
