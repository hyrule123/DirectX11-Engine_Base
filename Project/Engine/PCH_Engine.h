#ifndef PCH_ENGINE
#define PCH_ENGINE

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>
#include <assert.h>

#include <array>
#include <vector>
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

#ifdef _DEBUG
#pragma comment(lib, "fbxsdk/lib/Debug/libfbxsdk-md.lib")

#pragma comment(lib, "PhysX/lib/Debug/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysX_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/lib/Debug/PhysXFoundation_64.lib")



#else
#pragma comment(lib, "fbxsdk/lib/Release/libfbxsdk-md.lib")


#pragma comment(lib, "PhysX/lib/Release/PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXVehicle_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXVehicle2_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXCharacterKinematic_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXCooking_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PVDRuntime_64.lib")
#pragma comment(lib, "PhysX/lib/Release/SceneQuery_static_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysX_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXCommon_64.lib")
#pragma comment(lib, "PhysX/lib/Release/PhysXFoundation_64.lib")

#endif

#endif


