#pragma once
#include <Engine/DefaultShader/Common_struct.hlsli>

#include <unordered_set>
#include <string>

//전역변수 및 전역함수 모음
namespace core
{
	extern tGPUInitSetting		g_GPU_init_setting;
	extern tCB_Global			g_global_cbuffer;
	
	//c_str 저장용 컨테이너
	extern std::unordered_set<std::string> g_string_archive;
}

