#pragma once
#include "Engine/Util/EnumFlags.h"
#include "Engine/Util/type_traits_Ex.h"

namespace ehw
{

	enum class eResult
	{
		//음수 = 성공
		//이 아래로 성공 관련 변수를 저장
		Success = -1,

		Fail_NotImplemented = 0,

		//양수 = 실패
		Fail,

		Fail_Open,
		Fail_Path,

		Fail_Create,

		Fail_Write,
		Fail_Read,

		Fail_Nullptr,

		Fail_InValid,

		Fail_Empty
	};
	inline bool eResultSuccess(eResult _result) { return ((int)_result < 0); }
	inline bool eResultFail(eResult _result) { return ((int)_result >= 0); }


	constexpr const unsigned int g_maxLayer = 32u;
	constexpr inline bool IsLayerValid(unsigned int _layer) { return (g_maxLayer > _layer); }
	template <type_traits_Ex::is_enum_class_v T>
	constexpr inline bool IsLayerValid(T _layer) { return (g_maxLayer > static_cast<unsigned int>(_layer)); }

	enum class eFrameTimeStep
	{
		Frame_240,
		Frame_144,
		Frame_120,
		Frame_60,
		Frame_45,
		Frame_30,
		Frame_15,
		Frame_10,
		Frame_5,
		Frame_1,

		END
	};
	static constexpr std::array<float, (int)eFrameTimeStep::END> g_frameTimeStep
	{
		1.f / 240.f,
		1.f / 144.f,
		1.f / 120.f,
		1.f / 60.f,
		1.f / 45.f,
		1.f / 30.f,
		1.f / 15.f,
		1.f / 10.f,
		1.f / 5.f,
		1.f,
	};

	inline static constexpr float GetFrameTimeStep(const eFrameTimeStep _timeStep)
	{
		return g_frameTimeStep[(int)_timeStep];
	}
	inline static constexpr float GetFrameTimeStep(const int _timeStep)
	{
		return g_frameTimeStep[_timeStep];
	}

	enum class eCollisionStatus
	{
		Enter,
		Stay,
		Exit
	};

	enum class eColliderShape
	{
		AABB,
		OBB,
		Circle,
		Cube,
		Sphere,
		Capsule,
		END
	};

	enum class eCollider2D_Shape
	{
		AABB,
		OBB,
		Circle,
		END
	};

	enum class eCollider3DType
	{
		Rigid,
		END
	};

	enum class eCollider3D_Shape
	{
		Cube,
		Sphere,
		Capsule,
		END,
	};

	//enum class eLayer
	//{
	//	None = -1,
	//	Com_Camera,
	//	Grid,
	//	Monster,
	//	Player,

	//	tParticle,
	//	PostProcess,
	//	UI,

	//	END
	//};
	//inline bool CheckeLayerValid(eLayer _type)
	//{
	//	return ((eLayer::None != _type) && (eLayer::END != _type));
	//}

	enum class eProjectionType
	{
		None = -1,
		Perspective,
		Orthographic,
	};




	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		END,
	};

	enum class eDimensionType
	{
		NOT_SET,
		_2D,
		_3D
	};


	enum class eRotationOrder
	{
		XYZ,
		XZY,
		YXZ,
		YZX,
		ZXY,
		ZYX,

		END
	};
}
