#pragma once
#include <string>
#include <filesystem>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

#if defined (_WIN64) || (_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif

#include <fbxsdk/core/base/fbxtime.h>

#include <Engine/define_Enum.h>
#include <Engine/define_Macro.h>

#include <Engine/Util/Singleton.h>
#include <Engine/Util/SimpleMath.h>
#include <Engine/DefaultShader/Common_struct.hlsli>
//SimpleMath, float2~4, int2~4, uint2~4, BOOL, TRUE, FALSE 등 정의
#include <Engine/DefaultShader/Common_define.hlsli>
#include <Engine/Util/type_traits_Ex.h>

//https://stackoverflow.com/questions/10170087/how-does-stdfunction-know-about-calling-convention
using WindowMsgHandleFunc = std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)>;

namespace std
{
	namespace fs = filesystem;
}

namespace core
{
	template <typename T>
	using s_ptr = std::shared_ptr<T>;
	template <typename T>
	using w_ptr = std::weak_ptr<T>;

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

	struct tGPUManagerDesc
	{
		UINT resolution_X;
		UINT resolution_Y;
		UINT refresh_rate;
	};

	struct tGameEngineDesc
	{
		HWND hwnd;
		int  window_left_pos;
		int	 window_top_pos;
		int  width;
		int  height;
		tGPUManagerDesc GPU_desc;
		WNDCLASSEX window_class_desc;

		std::function<void()> editor_run_func;
	};

	struct Hasher_StringView {
		using hash_type = std::hash<std::string_view>;
		using is_transparent = void;

		std::size_t operator()(const char* str) const { return hash_type{}(str); }
		std::size_t operator()(const std::string_view str) const { return hash_type{}(str); }
		std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
	};

	template <typename T>
	using umap_string = std::unordered_map<std::string, T, Hasher_StringView, std::equal_to<>>;

	struct tMTBone
	{
		std::string			name;
		int					depth_level;
		int					parent_idx;
		MATRIX				offset_matrix;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
		MATRIX				bone_matrix;	// 이거 안씀
	};


	class ColliderID
	{
	public:
		ColliderID() = delete;
		ColliderID(uint32 _1, uint32 _2);
		~ColliderID();

		struct tID32Pair
		{
			//ID중 낮은쪽
			uint32 lower;

			//ID중 높은쪽
			uint32 higher;
		};
		union uID
		{
			tID32Pair ID32_pair;
			std::uint64_t ID64;
		};

		uID m_ID;

		operator uint64() const { return m_ID.ID64; }
	};

	struct ColliderID_Hasher
	{
		static_assert(sizeof(ColliderID) == sizeof(size_t), "사이즈 미일치. 처리함수를 만드세요.");
		size_t operator()(ColliderID _id) const { return static_cast<size_t>(_id.m_ID.ID64); }
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
		s_ptr<Mesh> mesh;
		s_ptr<Material> material;
		std::vector<s_ptr<GameObject>> objects_to_render;
	};
}
