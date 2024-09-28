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
		_0,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,

		//명칭
		diffuse_texture = GPU::Register::t::g_diffuse_texture,
		normal_texture = GPU::Register::t::g_normal_texture,
		specular_texture = GPU::Register::t::g_specular_texture,
		emissive_texture = GPU::Register::t::g_emissive_texture,
		roughness_metalic_texture = GPU::Register::t::g_roughness_metalic_texture,

		//렌더타겟을 "텍스처"로 바인딩할때
		rendertarget_diffuse = GPU::Register::t::g_diffuse_rendertarget,
		rendertarget_specular = GPU::Register::t::g_specular_rendertarget,
		rendertarget_normal = GPU::Register::t::g_normal_rendertarget,
		rendertarget_view_position = GPU::Register::t::g_view_position_rendertarget,
		rendertarget_emissive = GPU::Register::t::g_emissive_rendertarget,
		rendertarget_roughness_metalic = GPU::Register::t::g_roughness_metalic_rendertarget,

		rendertarget_diffuse_light = GPU::Register::t::g_diffuse_light_rendertarget,
		rendertarget_specular_light = GPU::Register::t::g_specular_light_rendertarget,

		//CubeT8,
		//CubeT9,

		//Array2DT10,
		//Array2DT11,

		END
	};

}
