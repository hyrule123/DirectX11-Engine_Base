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
		~TimeManager();

	public:
		__forceinline float DeltaTime() { return m_currentDeltaTime; }
		__forceinline float get_fixed_deltatime() { return m_fixedDeltaTime; }

		void update();
		
		uint GetFixedUpdateCount();
		inline void SetFixedUpdateMode(bool _enable);

		void RenderFPS();
		
		//0으로 설정시 실시간
		void SetFixedDeltaTime(float _fixedDeltaTime);

		//최대 fixed_update 횟수를 설정할 수 있음.
		//UINT_MAX로 설정시 무제한
		void SetMaxFixedUpdatesPerFrame(uint _max);

	private:
		float			m_currentDeltaTime;

		//DeltaTime 측정 관련
		float			m_deltaTime;
		float			m_delatTime_max_cap_per_frame;

		std::chrono::steady_clock::time_point m_prevTime;
		std::chrono::steady_clock::time_point m_currentTime;
		//


		// fixed_update 관련
		float			m_fixedDeltaTime;
		uint				m_maxFixedUpdatesPerFrame;

		float			m_accumulatedDeltaTime;
		//

		float			mOneSecond;
	};

	inline void TimeManager::SetFixedUpdateMode(bool _enable)
	{
		if (_enable && (m_fixedDeltaTime != 0.f))
		{
			m_currentDeltaTime = m_fixedDeltaTime;
		}
		else
		{
			m_currentDeltaTime = m_deltaTime;
		}
	}
}


