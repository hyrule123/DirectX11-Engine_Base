#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <memory>

#if defined (_WIN64) || (_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#include <fbxsdk/core/base/fbxtime.h>

#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"

#include "Engine/Util/Singleton.h"
#include "Engine/Util/SimpleMath.h"
#include "Engine/DefaultShader/Common_struct.hlsli"
//SimpleMath, float2~4, int2~4, uint2~4, BOOL, TRUE, FALSE 등 정의
#include "Engine/DefaultShader/Common_define.hlsli"
#include "Engine/Util/type_traits_Ex.h"

//https://stackoverflow.com/questions/10170087/how-does-stdfunction-know-about-calling-convention
using WindowMsgHandleFunc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

namespace std
{
	namespace fs = filesystem;
}

namespace core
{
	class GameObject;
	class Mesh;
	class Material;

	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using uint = unsigned int;
	using GameObjects = std::vector<std::unique_ptr<GameObject>>;

	struct tGPUManagerDesc
	{
		UINT ResolutionX;
		UINT ResolutionY;
		UINT RefreshRate;
	};

	struct tGameEngineDesc
	{
		HWND Hwnd;
		int  LeftWindowPos;
		int	 TopWindowPos;
		int  Width;
		int  Height;
		tGPUManagerDesc GPUDesc;
		WNDCLASSEX WinClassDesc;

		std::function<void()> EditorRunFunction;
	};

	struct Hasher_StringView {
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return hash_type{}(str); }
		std::size_t operator()(const std::string_view str) const { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};

	//Shared_ptr은 std::hash에 자체적으로 .get 해서 해싱을 하는 기능이 있음(굳이 내가 따로 선언할 필요가 없음)
	struct tDataPtr
	{
		void* pData;
		size_t size;

		template <typename T>
		void SetDataPtr(const T& _pData) { pData = (void*)_pData; size = sizeof(T); }
	};


	struct tMTBone
	{
		std::string			strBoneName;
		int					iDepth;
		int					iParentIndx;
		MATRIX				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
		MATRIX				matBone;	// 이거 안씀
	};


	struct tColliderID
	{
		struct tIDPair
		{
			//ID중 낮은쪽
			uint32 Low32;

			//ID중 높은쪽
			uint32 High32;
		};

		union
		{
			tIDPair pair;
			std::uint64_t ID;
		} ID64;

		tColliderID() = delete;
		tColliderID(uint32 _1, uint32 _2);
		~tColliderID();
		operator uint64() const { return ID64.ID; }
	};

	struct tColliderID_Hasher
	{
		static_assert(sizeof(tColliderID) == sizeof(size_t), "사이즈 미일치. 처리함수를 만드세요.");
		size_t operator()(tColliderID _id) const { return static_cast<size_t>(_id.ID64.ID); }
	};

	struct tDebugMesh
	{
		//eColliderType type;
		float3 position;
		Quaternion rotation;
		float3 scale;

		float radius;
		float duration;
		float time;
	};


	struct tRenderQueue {
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		std::vector<GameObject*> objects_to_render;
	};
}
