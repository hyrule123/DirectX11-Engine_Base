#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Engine/define_Enum.h"

#include "Engine/CommonType.h"
#include "Engine/Util/SimpleMath.h"
#include "Engine/DefaultShader/CommonStruct.hlsli"

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <variant>

#include <fbxsdk/core/base/fbxtime.h>

//https://stackoverflow.com/questions/10170087/how-does-stdfunction-know-about-calling-convention
using WindowMsgHandleFunc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

//C++ 전용. 1개 이상의 클래스에서 사용되는 "공용" 구조체 모음
namespace ehw
{


	struct tDesc_GPUMgr
	{
		UINT ResolutionX;
		UINT ResolutionY;
		UINT RefreshRate;
	};

	struct tDesc_GameEngine
	{
		HWND Hwnd;
		int  LeftWindowPos;
		int	 TopWindowPos;
		int  Width;
		int  Height;
		tDesc_GPUMgr GPUDesc;
		WNDCLASSEX WinClassDesc;
	};

	template<typename T>
	struct tFastHashFunc
	{
		std::size_t operator()(const T& _uKey) const
		{
			return static_cast<std::size_t>(_uKey);
		}
	};

	struct tHashFunc_StringView
	{
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return hash_type{}(str); }
		std::size_t operator()(const std::string_view str) const { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};

	//Shared_ptr은 std::hash에 자체적으로 .get 해서 해싱을 하는 기능이 있음(굳이 내가 따로 선언할 필요가 없음)
	//template <typename T>
	//struct tHashFunc_SharedPtr
	//{
	//	std::size_t operator()(const std::shared_ptr<T>& _sPtr) const
	//	{
	//		return reinterpret_cast<uint64>(_sPtr.get());
	//	}
	//};

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

	struct tColliderID
	{
		struct tIDPair
		{
			//ID중 낮은쪽
			std::uint32_t Low32;

			//ID중 높은쪽
			std::uint32_t High32;
		};

		union
		{
			tIDPair pair;
			std::uint64_t ID;
		} ID64;

		tColliderID() = delete;
		tColliderID(std::uint32_t _1, std::uint32_t _2);
		~tColliderID();
		operator std::uint64_t() const { return ID64.ID; }
	};

	struct tColliderID_Hasher
	{
		static_assert(sizeof(tColliderID) == sizeof(std::size_t), "사이즈 미일치. 처리함수를 만드세요.");
		std::size_t operator()(tColliderID _id) const { return static_cast<std::size_t>(_id.ID64.ID); }
	};

	struct tCollisionInfo
	{
		eCollisionStatus Status;
		float2 HitPoint;
		LARGE_INTEGER CheckedTime;
	};
};
