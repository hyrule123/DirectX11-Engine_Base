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


	constexpr const uint32_t g_maxLayer = 32;
	constexpr inline bool CheckLayerValid(uint32_t _layer) { return (g_maxLayer > _layer); }
	template <type_traits_Ex::is_enum_class_v T>
	constexpr inline bool CheckLayerValid(T _layer) { return (g_maxLayer > static_cast<uint32_t>(_layer)); }
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

	enum class eColliderType
	{
		None,
		Rect,
		Circle,
		Box,
		Sphere,
		END,
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

}
