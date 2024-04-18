#pragma once
#include <chrono>

namespace ehw
{
	class TimeManager
	{
		friend class GameEngine;
	public:
		static __forceinline float DeltaTime() { return m_deltaTime; }

		static void Update();
		static void Render(HDC _hdc);

	private:
		static void Init();
		static void Release();

	private:
		static float	m_deltaTime;

		static std::chrono::steady_clock::time_point m_prevTime;
		static std::chrono::steady_clock::time_point m_currentTime;

		static float			mOneSecond;

	private:
		TimeManager() = delete;
		~TimeManager() = delete;
	};
}


