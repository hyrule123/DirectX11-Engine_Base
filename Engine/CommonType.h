#pragma once
//SimpleMath, float2~4, int2~4, uint2~4, BOOL, TRUE, FALSE 등 정의
#include "Engine/DefaultShader/Common.hlsli"

#include <string>

#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}

namespace ehw
{
	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	using uint = unsigned int;
}
