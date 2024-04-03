#pragma once
#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"
#include "Engine/Util/SimpleMath.h"
#include <PhysX/PxPhysicsAPI.h>

namespace ehw
{
	class MathUtil
	{
	public:
		static inline  Quaternion	 ToQuaternion(const physx::PxQuat& data);
		static inline physx::PxQuat ToPxQuat(const Quaternion& quaternion);

		static inline float3		 Tofloat3(const physx::PxVec3& data);
		static inline physx::PxVec3  ToPxVec3(const float3& vector);

		static inline MATRIX		  ToMATRIX(const physx::PxMat44& data);
		static inline physx::PxMat44 ToPxMat44(const MATRIX& matrix);

		static Quaternion MakeRotationByASF(float x, float y, float z, eRotationOrder order);

	private:
		MathUtil() = delete;
		~MathUtil() = delete;
	};

	inline Quaternion MathUtil::ToQuaternion(const physx::PxQuat& data)
	{
		return Quaternion{ data.x, data.y, data.z, data.w };
	}
	inline physx::PxQuat MathUtil::ToPxQuat(const Quaternion& quaternion)
	{
		return physx::PxQuat{ quaternion.x, quaternion.y, quaternion.z, quaternion.w };
	}
	inline float3 MathUtil::Tofloat3(const physx::PxVec3& data)
	{
		return float3{ data.x, data.y, data.z };
	}
	inline physx::PxVec3 MathUtil::ToPxVec3(const float3& vector)
	{
		return physx::PxVec3{ vector.x, vector.y, vector.z };
	}
	inline MATRIX MathUtil::ToMATRIX(const physx::PxMat44& data)
	{
		MATRIX result{};
		std::memcpy(&result, &data, sizeof(MATRIX));
		return result;
	}
	inline physx::PxMat44 MathUtil::ToPxMat44(const MATRIX& matrix)
	{
		physx::PxMat44 result{};
		std::memcpy(&result, &matrix, sizeof(physx::PxMat44));
		return result;
	}
}

