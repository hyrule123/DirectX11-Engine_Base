#pragma once
#include "Engine/DefaultShader/CommonStruct.hlsli"

#include <unordered_set>
#include <string>

namespace ehw
{
	extern tGPUInitSetting		gGPUInitSetting;
	extern tCB_Global			gGlobal;
	
	//c_str 저장용 컨테이너
	extern std::unordered_set<std::string> g_stringArchive;

	constexpr float g_deltaTimeMaxCap = 1.f / 30.f;
}

