#pragma once

namespace ehw
{
	class TimeManager
	{
		friend class GameEngine;
	public:
		static __forceinline float DeltaTime() { return mDeltaTime; }
		static __forceinline LARGE_INTEGER CurrentFrenquency() { return mCurFrequency; }

		static void Update();
		static void Render(HDC _hdc);

	private:
		static void Init();
		static void Release();

	private:
		static float	mDeltaTime;

		static LARGE_INTEGER	mCpuFrequency;
		static LARGE_INTEGER    mPrevFrequency;
		static LARGE_INTEGER	mCurFrequency;
		static float			mOneSecond;

	private:
		TimeManager() = delete;
		~TimeManager() = delete;
	};
}


