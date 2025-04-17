#ifndef PCH_ENGINE
#define PCH_ENGINE



#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <commdlg.h>
#include <combaseapi.h>

#include <string>
#include <array>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <bitset>
#include <set>
#include <functional>
#include <unordered_set>

#include <cmath>
#include <algorithm>
#include <utility>
#include <limits>
#include <memory>
#include <filesystem>
#include <fstream>
#include <span>

#include <thread>
#include <chrono>
#include <future>
#include <cassert>

#include "Engine/Util/EnumFlags.h"
#include "Engine/Util/SimpleMath.h"
#include "Engine/Util/Serialize/json.h"

//PhysX
#pragma comment(lib, "Physx/PhysX.lib")
#pragma comment(lib, "Physx/PhysXCharacterKinematic_static.lib")
#pragma comment(lib, "Physx/PhysXCommon.lib")
#pragma comment(lib, "Physx/PhysXCooking.lib")
#pragma comment(lib, "Physx/PhysXExtensions_static.lib")
#pragma comment(lib, "Physx/PhysXFoundation.lib")
#pragma comment(lib, "Physx/PhysXPvdSDK_static.lib")
#pragma comment(lib, "Physx/PhysXTask_static.lib")
#pragma comment(lib, "Physx/PhysXVehicle_static.lib")
#pragma comment(lib, "Physx/PhysXVehicle2_static.lib")
#pragma comment(lib, "Physx/PVDRuntime.lib")
#pragma comment(lib, "Physx/SceneQuery_static.lib")
#pragma comment(lib, "Physx/SimulationController_static.lib")
#pragma comment(lib, "Physx/LowLevel_static.lib")
#pragma comment(lib, "Physx/LowLevelAABB_static.lib")
#pragma comment(lib, "Physx/LowLevelDynamics_static.lib")

//FMOD
#ifdef _DEBUG
#pragma comment(lib, "Fmod/fmodL_vc.lib")
#pragma comment(lib, "Fmod/fmodstudioL_vc.lib")
#else
#pragma comment(lib, "Fmod/fmod_vc.lib")
#pragma comment(lib, "Fmod/fmodstudio_vc.lib")
#endif 

//FBX
#pragma comment(lib, "fbxsdk/libfbxsdk-md.lib")

#endif


