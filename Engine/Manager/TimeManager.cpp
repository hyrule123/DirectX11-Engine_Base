
#include "TimeManager.h"

#include <Engine/Util/AtExit.h>
#include <Engine/GameEngine.h>

#include <Engine/Global.h>

#include <Engine/define_Macro.h>

namespace core
{
    TimeManager::TimeManager() 
        : m_cur_delta_time{}
        , m_delta_time{}
        , m_delta_time_cap_per_frame{}
        , m_prev_time_point{}
        , m_current_time_stamp{}
        , m_fixed_delta_time{}
        , m_max_fixed_updates_per_frame{}
        , m_accumulated_delta_time{}
        , m_one_second{}
    {
    }
    void TimeManager::init()
    {
        m_delta_time_cap_per_frame = g_default_deltatime_max_cap;
        m_prev_time_point = std::chrono::high_resolution_clock::now();
        m_fixed_delta_time = g_defaultFixedUpdateDeltaTime;
        m_max_fixed_updates_per_frame = g_maxFixedUpdatesPerFrame;

        set_max_fixed_updates_per_frame(g_maxFixedUpdatesPerFrame);
        set_fixed_delta_time(g_defaultFixedUpdateDeltaTime);

        AtExit::add_func(TimeManager::destroy_inst);
    }


    TimeManager::~TimeManager()
    {

    }

    void TimeManager::update()
    {
        //DeltaTime 계산
        {
            m_current_time_stamp = std::chrono::high_resolution_clock::now();

            std::chrono::duration<float> duration = m_current_time_stamp - m_prev_time_point;
            m_delta_time = duration.count();

            if (g_default_deltatime_max_cap < m_delta_time)
            {
                m_delta_time = g_default_deltatime_max_cap;
            }

            m_prev_time_point = m_current_time_stamp;
        }

        m_cur_delta_time = m_delta_time;


        //static std::chrono::steady_clock::time_point end{};
        //static std::chrono::steady_clock::time_point start{};
        

        //if (m_fixedUpdateCount)
        //{
        //    end = std::chrono::high_resolution_clock::now();
        //    
        //    std::stringstream stream{};
        //    static std::chrono::duration<float> dur = end - start;


        //    stream << std::to_string(get_fixed_deltatime()) << "\n";
        //    DEBUG_LOG_A(stream.str().c_str());
        //}
        //else
        //{
        //    start = std::chrono::high_resolution_clock::now();
        //}
    }

    uint TimeManager::get_fixed_update_count()
    {
        //fixed_update 횟수 계산
        uint count = 0u;

        if (m_fixed_delta_time == 0.f)
        {
            count = 1u;
            m_accumulated_delta_time = 0.f;
        }
        else
        {
            m_accumulated_delta_time += m_delta_time;
            count = (uint)(m_accumulated_delta_time / m_fixed_delta_time);

            m_accumulated_delta_time -= ((float)count * m_fixed_delta_time);
            if (m_accumulated_delta_time < 0.f)
            {
                m_accumulated_delta_time = 0.f;
            }

            count = m_max_fixed_updates_per_frame < count ? m_max_fixed_updates_per_frame : count;
        }

        return count;
    }



    void TimeManager::render_FPS()
    {
#ifdef _DEBUG
        static int iCount = 0;
        ++iCount;

        // 1 초에 한번
        m_one_second += m_delta_time;
        if (1.0f < m_one_second)
        {
            HWND hWnd = GameEngine::get_inst().get_HWND();

            wchar_t szFloat[50] = {};
            float FPS = 1.f / m_delta_time;
            swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            m_one_second = 0.f;
            iCount = 0;
        }
#endif //_DEBUG


    }
    void TimeManager::set_fixed_delta_time(float _fixedDeltaTime)
    {
        ASSERT(0.f < _fixedDeltaTime, "Fixed Delta Time은 음수가 될 수 없음.");

        m_fixed_delta_time = _fixedDeltaTime;
    }
    void TimeManager::set_max_fixed_updates_per_frame(uint _max)
    {
        ASSERT(0u < _max, "0으로는 설정할 수 없습니다.");

        m_max_fixed_updates_per_frame = _max;
    }
}
