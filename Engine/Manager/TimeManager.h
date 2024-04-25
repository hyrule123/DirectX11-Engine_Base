#pragma once
#include "Engine/define_Enum.h"
#include "Engine/DefaultSettingVar.h"
#include "Engine/GlobalVariable.h"

#include <chrono>

namespace ehw
{
	class TimeManager
	{
		friend class GameEngine;
	public:
		static __forceinline float DeltaTime() { return m_currentDeltaTime; }
		static __forceinline float FixedDeltaTime() { return m_fixedDeltaTime; }

		static void Update();
		
		static uint GetFixedUpdateCount();
		inline static void SetFixedUpdateMode(bool _enable);

		static void RenderFPS();
		
		//0으로 설정시 실시간
		static void SetFixedDeltaTime(float _fixedDeltaTime);

		//최대 FixedUpdate 횟수를 설정할 수 있음.
		//UINT_MAX로 설정시 무제한
		static void SetMaxFixedUpdatesPerFrame(uint _max);

	private:
		static void Init();
		static void Release();

	private:
		static float			m_currentDeltaTime;

		//DeltaTime 측정 관련
		static float			m_deltaTime;
		static float			m_delatTime_max_cap_per_frame;

		static std::chrono::steady_clock::time_point m_prevTime;
		static std::chrono::steady_clock::time_point m_currentTime;
		//


		// FixedUpdate 관련
		static float			m_fixedDeltaTime;
		static uint				m_maxFixedUpdatesPerFrame;

		static float			m_accumulatedDeltaTime;
		//

		static float			mOneSecond;

	private:
		TimeManager() = delete;
		~TimeManager() = delete;
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


