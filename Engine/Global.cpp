#include "Global.h"

namespace core
{
	tGPUInitSetting g_GPU_init_setting{};
	tCB_Global		g_global_cbuffer{};
	int g_instance_count = 0;
	std::unordered_set<std::string> g_string_archive{};
}
