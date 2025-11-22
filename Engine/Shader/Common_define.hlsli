#ifndef COMMON_HLSLI
#define COMMON_HLSLI

//C++ - HLSL 자료형 일치를 위한 헤더
#ifndef __cplusplus
#define __HLSL
#endif

//C++
#ifdef __cplusplus
#include <cstdint>
#include <Engine/Util/Simplemath.h>


using float2 = core::math::Vector2;
using core::math::Vector2;
using float3 = core::math::Vector3;
using core::math::Vector3;
using float4 = core::math::Vector4;
using core::math::Vector4;
using core::math::Quaternion;
using MATRIX = core::math::Matrix;
using uint = std::uint32_t;

//윈도우 쪽 헤더에 정의되어 있음
#ifndef BOOL
using BOOL = int;
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

struct int2 { int x; int y; };
struct int3 { int x; int y; int z; };
struct int4 { int x; int y; int z; int w; };

using uint = std::uint32_t;
struct uint2 { uint x; uint y; };
struct uint3 { uint x; uint y; uint z; };
struct uint4 { uint x; uint y; uint z; uint w; };

struct uintx8 { uint u[8]; };
struct uintx16 { uint u[16]; };



#define SEMANTIC(_Type)

#else
//=========================================
//================ HLSL ===================
//=========================================

#define FLT_EPSILON 1.192092896e-07F

#define BOOL int
#define TRUE 1
#define FALSE 0
#define MATRIX row_major matrix

#define uintx8 row_major uint2x4
#define uintx16 row_major uint4x4

#define SEMANTIC(_Type) : _Type

#endif

//ENUM 관련
#ifdef __cplusplus

//C++ : enum class 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) enum class _Name : _Type {
#define ENUM_MEMBER(_Name, _Type, _Val) _Name = _Val,
#define ENUM_END };

#else

//HLSL : namespace 형태로 선언함.
#define ENUM_BEGIN(_Name, _Type) namespace _Name {
#define ENUM_MEMBER(_Name, _Type, _Val) static const _Type _Name = _Val;
#define ENUM_END };

#endif


//Register Slot 관련
#ifdef __cplusplus


#define REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER)\
namespace GPU::Register::REGISTER_TYPE {\
constexpr int BUFFER_NAME = REGISTER_NUMBER;\
}

#define CBUFFER(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#define TEXTURE2D(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#define TEXTURE2D_RW(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER, _format)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#define SBUFFER(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#define SBUFFER_RW(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#define SAMPLER(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER)\
REGISTER_DECLARE(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER);\

#else//HLSL

#define CBUFFER(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
cbuffer BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)\
{ STRUCT_TYPE BUFFER_NAME; }

#define TEXTURE2D(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER)\
Texture2D BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)

#define TEXTURE2D_RW(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER, _format)\
RWTexture2D<_format> BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)

#define SBUFFER(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
StructuredBuffer<STRUCT_TYPE> BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)

#define SBUFFER_RW(BUFFER_NAME, STRUCT_TYPE, REGISTER_TYPE, REGISTER_NUMBER)\
RWStructuredBuffer<STRUCT_TYPE> BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)

#define SAMPLER(BUFFER_NAME, REGISTER_TYPE, REGISTER_NUMBER)\
SamplerState BUFFER_NAME : register(REGISTER_TYPE##REGISTER_NUMBER)

#define alignas(_Num) 

#endif

#endif



