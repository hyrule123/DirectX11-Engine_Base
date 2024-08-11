#pragma once
#include "Engine/DefaultShader/Common_struct.hlsli"

#include <unordered_set>
#include <string>

namespace ehw
{
	extern tGPUInitSetting		gGPUInitSetting;
	extern tCB_Global			gGlobal;
	
	//c_str 저장용 컨테이너
	extern std::unordered_set<std::string> g_stringArchive;
}

