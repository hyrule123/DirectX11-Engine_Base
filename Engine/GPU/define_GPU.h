#pragma once
#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"


namespace core
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

	namespace name
	{
		STR_KEY ArrGSPrefix[(int)eGSStage::END] =
		{
			"VS_",
			"HS_",
			"DS_",
			"GS_",
			"PS_",
		};
		STR_KEY CSPrefix = "CS_";
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

	enum class eRasterizerState
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		END,
	};

	enum class eDepthStencilState
	{
		Less,
		Greater,
		NoWrite,
		None,
		END,
	};

	enum class eBlendState
	{
		Default,
		AlphaBlend,
		OneOne,
		END,
	};

	namespace name
	{
		constexpr const char* eSamplerType[(int)core::eSamplerType::END]
		{
			"Point",
			"Linear",
			"Anisotropic",
		};

		constexpr const char* eRasterizerState[(int)core::eRasterizerState::END]
		{
			"SolidBack",
			"SolidFront",
			"SolidNone",
			"WireframeNone",
		};

		constexpr const char* eDepthStencilState[(int)core::eDepthStencilState::END]
		{
			"Less",
			"Greater",
			"NoWrite",
			"None",
		};

		constexpr const char* eBlendState[(int)core::eBlendState::END]
		{
			"Default",
			"AlphaBlend",
			"OneOne",
		};
	}

	enum class eRenderingMode
	{
		NONE = -1,

		//Deffered
		deffered_opaque,	//불투명 오브젝트
		deffered_mask,	//무조건 덮어쓰는 오브젝트

		//광원처리
		//Light,

		//Forward
		forward_opaque,
		forward_cutout,
		forward_transparent,
		post_process,
		
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


	enum class eBufferViewType
	{
		NONE,
		SRV,
		UAV,
		RTV,
		DSV,
	};
}
