#pragma once
#include "Engine/CommonType.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	constexpr float g_default_deltatime_max_cap = 1.f / 15.f;
	constexpr uint g_default_FixedUpdate_max_count_per_frame = 5u;

	constexpr float g_defaultDensity = 10.f;

	constexpr uint g_maxFixedUpdatesPerFrame = 4;
	constexpr float g_defaultFixedUpdateDeltaTime = GetFrameTimeStep(eFrameTimeStep::Frame_60);
	constexpr eFrameTimeStep g_defaultRefreshRate = eFrameTimeStep::Frame_60;
}
