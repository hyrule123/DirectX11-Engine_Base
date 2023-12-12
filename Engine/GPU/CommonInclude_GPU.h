#pragma once
#include "define_GPU.h"


#include "../DefaultShader/ConstBuffer.hlsli"
#include "../DefaultShader/Resource.hlsli"


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

	END = SpecularLightTarget + 1,
};
