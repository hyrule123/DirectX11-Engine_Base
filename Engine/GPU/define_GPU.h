#pragma once
#include "../define_Enum.h"
#include "../define_Macro.h"


#include "../DefaultShader/ConstBuffer.hlsli"
#include "../DefaultShader/Resource.hlsli"


#include "../define_Enum.h"
#include "../define_Macro.h"


#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include <wrl.h>
using Microsoft::WRL::ComPtr;
namespace Microsoft::WRL
{
	template <class T> struct is_ComPtr : std::false_type {};
	template <class T> struct is_ComPtr<Microsoft::WRL::ComPtr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_ComPtr_v = is_ComPtr<T>::value;
}


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

		STRKEY Ext_ShaderSetting = ".json";
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

	enum class eTextureSlot
	{
		Tex_0,
		Tex_1,
		Tex_2,
		Tex_3,
		Tex_4,
		Tex_5,
		Tex_6,
		Tex_7,


		Albedo = Register_t_AlbedoTexture,
		Normal = Register_t_NormalTexture,
		Specular = Register_t_SpecularTexture,
		Emissive = Register_t_EmissiveTexture,
		RoughnessAndMetalic = Register_t_RoughnessMetalicTexture,

		AlbedoTarget = Register_t_AlbedoTarget,
		NormalTarget = Register_t_NormalTarget,
		SpecularTarget = Register_t_SpecularTarget,
		EmissiveTarget = Register_t_EmissiveTarget,
		RoughnessAndMetalicTarget = Register_t_RoughnessMetalicTarget,
		PositionTarget = Register_t_PositionTarget,
		
		DiffuseLightTarget = Register_t_DiffuseLightTarget,
		SpecularLightTarget = Register_t_SpecularLightTarget,

		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		END = 8,
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

	struct tDebugMesh
	{
		eColliderType type;
		float3 position;
		float3 rotatation;
		float3 scale;

		float radius;
		float duration;
		float time;
	};

	//struct tLightAttribute
	//{
	//	float4 diffuse;
	//	float4 specular;
	//	float4 ambient;

	//	float4 position;
	//	float4 direction;

	//	eLightType type;
	//	float radius;
	//	float angle;
	//	int padding;
	//};

	//struct tParticle
	//{
	//	float4 position;
	//	float4 direction;

	//	float lifeTime;
	//	float time;
	//	float speed;
	//	uint active;
	//};
	//struct tParticleShared
	//{
	//	uint activeCount;
	//};


}
