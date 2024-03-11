#pragma once
#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"


namespace ehw
{
	constexpr const int MRT_MAX = 8;

	enum class eValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};

	enum class eGSStage
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		END
	};

	namespace strKey
	{
		STRKEY ArrGSPrefix[(int)eGSStage::END] =
		{
			"VS_",
			"HS_",
			"DS_",
			"GS_",
			"PS_",
		};
		STRKEY CSPrefix = "CS_";
	}


	enum class eMRTType
	{
		Swapchain,
		Deffered,
		Light,
		Shadow,
		END,
	};


	enum class eShaderStage
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Pixel,
		Compute,
		ALL,
		END
	};

	namespace eShaderStageFlag
	{
		enum Flag
		{
			NONE = 0,
			Vertex = BIT_MASK(0),
			Hull = BIT_MASK(1),
			Domain = BIT_MASK(2),
			Geometry = BIT_MASK(3),
			Pixel = BIT_MASK(4),
			Compute = BIT_MASK(5),
			ALL = Vertex | Hull | Domain | Geometry | Pixel | Compute,
		};
	}
	using eShaderStageFlag_ = int;

	namespace SHADER_VERSION
	{
		constexpr const char* Graphics[(int)eShaderStage::END]
			= {
			"vs_5_0",
			"hs_5_0",
			"ds_5_0",
			"gs_5_0",
			"ps_5_0"
		};
		constexpr const char* Compute = "cs_5_0";
	}

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		END,
	};

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		END,
	};

	enum class eDSType
	{
		Less,
		Greater,
		NoWrite,
		None,
		END,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		END,
	};

	namespace strKey
	{
		constexpr const char* eSamplerType[(int)ehw::eSamplerType::END]
		{
			"Point",
			"Linear",
			"Anisotropic",
		};

		constexpr const char* eRSType[(int)ehw::eRSType::END]
		{
			"SolidBack",
			"SolidFront",
			"SolidNone",
			"WireframeNone",
		};

		constexpr const char* eDSType[(int)ehw::eDSType::END]
		{
			"Less",
			"Greater",
			"NoWrite",
			"None",
		};

		constexpr const char* eBSType[(int)ehw::eBSType::END]
		{
			"Default",
			"AlphaBlend",
			"OneOne",
		};
	}

	enum class eRenderingMode
	{
		None = -1,

		//Deffered
		DefferdOpaque,
		DefferdMask,

		//광원처리
		Light,

		//Forward
		Opaque,
		CutOut,
		Transparent,
		PostProcess,
		
		END,
	};





	enum class eGPUParam
	{
		Int,
		Float,
		float2,
		float3,
		float4,
		MATRIX,
	};

	enum class eSRVType
	{
		SRV,
		UAV,
		END,
	};

	enum class eMRT_Deffered
	{
		AlbedoTarget,
		NormalTarget,
		SpecularTarget,
		EmissiveTarget,
		RoughnessAndMetalicTarget,
		PositionTarget,
		END
	};
	namespace strKey
	{
		STRKEY eMRT_Deffered_String[(int)eMRT_Deffered::END]
		{
			"AlbedoTarget",
			"NormalTarget",
			"SpecularTarget",
			"EmissiveTarget",
			"RoughnessAndMetalicTarget",
			"PositionTarget",
		};
	}

	enum class eMRT_Light
	{
		DiffuseLightTarget,
		SpecularLightTarget,
		END
	};
	namespace strKey
	{
		STRKEY eMRT_Light_String[(int)eMRT_Light::END]
		{
			"DiffuseLightTarget",
			"SpecularLightTarget",
		};
	}

	enum class eBufferViewType
	{
		NONE,
		SRV,
		UAV,
		RTV,
		DSV,
	};
}
