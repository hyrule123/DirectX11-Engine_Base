#include "Engine/GlobalVariable.h"

namespace ehw
{
	tGPUInitSetting gGPUInitSetting{};
	tCB_Global		gGlobal{};
	int g_instance_count = 0;
	std::unordered_set<std::string> g_stringArchive{};
}
