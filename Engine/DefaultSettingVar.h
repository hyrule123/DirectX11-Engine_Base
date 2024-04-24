#pragma once
#include "Engine/CommonType.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	constexpr float g_default_deltatime_max_cap = 1.f / 15.f;
	constexpr uint g_default_FixedUpdate_max_count_per_frame = 5u;
	constexpr eFrameTimeStep g_default_FixedUpdate_TimeStep = eFrameTimeStep::Frame_60;

	constexpr float g_defaultDensity = 10.f;
}
