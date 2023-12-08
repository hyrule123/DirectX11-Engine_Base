#ifndef PCH_ENGINE
#define PCH_ENGINE

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>
#include <assert.h>

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

#include <combaseapi.h>
#include "EnumFlags.h"

#include <PhysX/PxPhysicsAPI.h>

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "fbxsdk/x64/Debug/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/x64/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysX_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/x64/Debug/PhysXFoundation_64.lib")


#else _DEBUG

#pragma comment(lib, "fbxsdk/x64/Release/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/x64/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/x64/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysX_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/x64/Release/PhysXFoundation_64.lib")

#endif _DEBUG

#else _WIN64
#error "x64만 지원합니다"
#endif _WIN64

#endif


