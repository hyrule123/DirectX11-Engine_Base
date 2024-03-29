#ifndef COMMON_HLSLI
#define COMMON_HLSLI

//C++ - HLSL 자료형 일치를 위한 헤더

#ifndef __cplusplus
#define __HLSL
#endif

//C++
#ifdef __cplusplus
#include <cstdint>
#include "Engine/Util/Simplemath.h"


using float2 = ehw::math::Vector2;
using ehw::math::Vector2;
using float3 = ehw::math::Vector3;
using ehw::math::Vector3;
using float4 = ehw::math::Vector4;
using ehw::math::Vector4;
using ehw::math::Quaternion;
using MATRIX = ehw::math::Matrix;
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

#define SEMANTIC(_Type)  : _Type

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

#define REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)\
constexpr int Register_##_registerType##_##_bufferName = _registerNumber

#define CBUFFER(_bufferName, _structName, _registerType, _registerNumber)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber);\
constexpr int CBUFFER_##_bufferName = _registerNumber

#define TEXTURE2D(_bufferName, _registerType, _registerNumber)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)

#define TEXTURE2D_RW(_bufferName, _registerType, _registerNumber, _format)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)

#define SBUFFER(_bufferName, _structName, _registerType, _registerNumber)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)

#define SBUFFER_RW(_bufferName, _structName, _registerType, _registerNumber)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)

#define SAMPLER(_bufferName, _registerType, _registerNumber)\
REGISTER_DECLARE(_bufferName, _registerType, _registerNumber)

//#define REGISTER(_Type)
//#define REGISTER_SLOT(_RegisterType, _iSlotIdx) _iSlotIdx


#else//HLSL

#define CBUFFER(_bufferName, _structName, _registerType, _registerNumber)\
cbuffer _bufferName : register(_registerType##_registerNumber)\
{ _structName _bufferName; }

#define TEXTURE2D(_bufferName, _registerType, _registerNumber)\
Texture2D _bufferName : register(_registerType##_registerNumber)

#define TEXTURE2D_RW(_bufferName, _registerType, _registerNumber, _format)\
RWTexture2D<_format> _bufferName : register(_registerType##_registerNumber)

#define SBUFFER(_bufferName, _structName, _registerType, _registerNumber)\
StructuredBuffer<_structName> _bufferName : register(_registerType##_registerNumber)

#define SBUFFER_RW(_bufferName, _structName, _registerType, _registerNumber)\
RWStructuredBuffer<_structName> _bufferName : register(_registerType##_registerNumber)

#define SAMPLER(_bufferName, _registerType, _registerNumber)\
SamplerState _bufferName : register(_registerType##_registerNumber)

#define alignas(_Num) 

#endif

#endif



