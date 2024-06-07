#ifndef PCH_ENGINE
#define PCH_ENGINE

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>
#include <combaseapi.h>

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
#if defined (_WIN64) && (_DEBUG)
#pragma comment(lib, "Physx/x64/Debug/PhysX_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXTask_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "Physx/x64/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/SimulationController_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/LowLevel_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/LowLevelAABB_static_64.lib")
#pragma comment(lib, "Physx/x64/Debug/LowLevelDynamics_static_64.lib")

#elif defined (_WIN64) && !(_DEBUG)

#pragma comment(lib, "Physx/x64/Release/PhysX_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXCommon_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXCooking_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXFoundation_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXTask_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/PVDRuntime_64.lib")
#pragma comment(lib, "Physx/x64/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/SimulationController_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/LowLevel_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/LowLevelAABB_static_64.lib")
#pragma comment(lib, "Physx/x64/Release/LowLevelDynamics_static_64.lib")
#endif

//FMOD
#if defined (_WIN64) && (_DEBUG)
#pragma comment(lib, "Fmod/x64/Debug/fmodL_vc.lib")
#pragma comment(lib, "Fmod/x64/Debug/fmodstudioL_vc.lib")
#endif //(_WIN64) && (_DEBUG)

#if defined (_WIN64) && (NDEBUG)
#pragma comment(lib, "Fmod/x64/Release/fmod_vc.lib")
#pragma comment(lib, "Fmod/x64/Release/fmodstudio_vc.lib")
#endif //(_WIN64) && (NDEBUG)

//FBX
#if defined (_WIN64) && (_DEBUG)
#pragma comment(lib, "fbxsdk/x64/Debug/libfbxsdk-md.lib")
#endif //(_WIN64) && (_DEBUG)

#if defined (_WIN64) && (NDEBUG)
#pragma comment(lib, "fbxsdk/x64/Release/libfbxsdk-md.lib")
#endif //(_WIN64) && (NDEBUG)


#endif


