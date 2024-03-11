#pragma once

#include "Engine/define_Enum.h"

#include "Engine/Util/SimpleMath.h"
#include "Engine/DefaultShader/CommonStruct.hlsli"

#include <fbxsdk/core/base/fbxtime.h>

#include <string>
#include <vector>
#include <Windows.h>


//C++ 전용. 1개 이상의 클래스에서 사용되는 "공용" 구조체 모음
namespace ehw
{
	struct tDesc_GPUMgr
	{
		UINT ResolutionX;
		UINT ResolutionY;
		UINT RefreshRate;
	};

	struct tDesc_Application
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
		UINT64 operator()(const T& _uKey) const
		{
			return static_cast<UINT64>(_uKey);
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
		eColliderType type;
		float3 position;
		Quaternion rotation;
		float3 scale;

		float radius;
		float duration;
		float time;
	};

}
