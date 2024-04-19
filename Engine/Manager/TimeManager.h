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
		static __forceinline float DeltaTime() { return m_deltaTime; }
		static __forceinline float FixedDeltaTime() { return GetFrameTimeStep(m_currentTimeStep); }

		static void Update();
		static void Render(HDC _hdc);

		static int GetFixedUpdateCount() { return m_fixedUpdateCount; }
		static void AdjustFixedUpdateTimeStep(const float _fixedUpdateDuration);

	private:
		static void Init();
		static void Release();

	private:
		static float			m_deltaTime;
		static float			m_delatTime_max_cap_per_frame;

		//
		static float			m_accumulatedDeltaTime;
		static eFrameTimeStep	m_minFixedUpdateTimeStep;	//사용자가 설정한 최소 TimeStep
		static eFrameTimeStep	m_currentTimeStep;			//조정된 TimeStep(실제로 사용)
		static int				m_fixedUpdateCount;
		static int				m_maxFixedUpdatesPerFrame;
		//

		static std::chrono::steady_clock::time_point m_prevTime;
		static std::chrono::steady_clock::time_point m_currentTime;

		static float			mOneSecond;

	private:
		TimeManager() = delete;
		~TimeManager() = delete;
	};
}


