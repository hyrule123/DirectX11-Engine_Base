#pragma once
#include <Engine/define_Enum.h>
#include <Engine/DefaultSettingVar.h>
#include <Engine/Global.h>
#include <Engine/Util/Singleton.h>

#include <chrono>

namespace core
{
	class TimeManager
	{
		friend class GameEngine;

		DECLARE_SINGLETON(TimeManager);
	private:
		TimeManager();
		void init();

		~TimeManager();

	public:
		__forceinline float delta_time() { return m_cur_delta_time; }
		__forceinline float get_fixed_deltatime() { return m_fixed_delta_time; }

		void update();
		
		uint get_fixed_update_count();
		inline void set_fixed_update_mode(bool _enable);

		void render_FPS();
		
		//0으로 설정시 실시간
		void set_fixed_delta_time(float _fixedDeltaTime);

		//최대 fixed_update 횟수를 설정할 수 있음.
		//UINT_MAX로 설정시 무제한
		void set_max_fixed_updates_per_frame(uint _max);

	private:
		float			m_cur_delta_time;

		//DeltaTime 측정 관련
		float			m_delta_time;
		float			m_delta_time_cap_per_frame;

		std::chrono::steady_clock::time_point m_prev_time_point;
		std::chrono::steady_clock::time_point m_current_time_stamp;
		//


		// fixed_update 관련
		float			m_fixed_delta_time;
		uint				m_max_fixed_updates_per_frame;

		float			m_accumulated_delta_time;
		//

		float			m_one_second;
	};

	inline void TimeManager::set_fixed_update_mode(bool _enable)
	{
		if (_enable && (m_fixed_delta_time != 0.f))
		{
			m_cur_delta_time = m_fixed_delta_time;
		}
		else
		{
			m_cur_delta_time = m_delta_time;
		}
	}
}


