#pragma once
#include "Engine/Common.h"
#include "Engine/GPU/define_GPU.h"
#include "Engine/DefaultShader/common_struct.hlsli"

#include "Engine/DefaultShader/ConstBuffer.hlsli"
#include "Engine/DefaultShader/Common_register.hlsli"

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
	//정점의 최소 데이터 -> 정점을 생성할때는 반드시 이 구조체를 상속받아서 사용
	struct VertexBase {
		float4 position;
	};
	
	enum class eTextureSlot
	{
		//기본 이름(최대 8장)
		Tex_0,
		Tex_1,
		Tex_2,
		Tex_3,
		Tex_4,
		Tex_5,
		Tex_6,
		Tex_7,

		//명칭
		Albedo = GPU::Register::t::AlbedoTexture,
		Normal = GPU::Register::t::NormalTexture,
		Specular = GPU::Register::t::SpecularTexture,
		Emissive = GPU::Register::t::EmissiveTexture,
		RoughnessAndMetalic = GPU::Register::t::RoughnessMetalicTexture,

		AlbedoTarget = GPU::Register::t::AlbedoTarget,
		NormalTarget = GPU::Register::t::NormalTarget,
		SpecularTarget = GPU::Register::t::SpecularTarget,
		EmissiveTarget = GPU::Register::t::EmissiveTarget,
		RoughnessAndMetalicTarget = GPU::Register::t::RoughnessMetalicTarget,
		PositionTarget = GPU::Register::t::PositionTarget,

		DiffuseLightTarget = GPU::Register::t::DiffuseLightTarget,
		SpecularLightTarget = GPU::Register::t::SpecularLightTarget,

		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		END
	};

}
