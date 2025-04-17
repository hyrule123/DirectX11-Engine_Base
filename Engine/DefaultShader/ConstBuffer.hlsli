#ifndef CONST_BUFFER_HLSLI
#define CONST_BUFFER_HLSLI

#include "Engine/DefaultShader/Common_struct.hlsli"


//////////////////////////////////
//새로운 상수버퍼 생성시 해줘야 할것
//구조체 선언
//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호) 작성
//아래 C++ enum class에 해당 번호 추가
//RenderMgr에서 상수버퍼 추가
/////////////////////////////////


//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호)
CBUFFER(g_CB_global, tCB_Global, b, 0);
CBUFFER(g_CB_camera, tCamera, b, 1);
CBUFFER(g_CB_material_shared_data, tSharedMaterialData, b, 2);
CBUFFER(g_CB_compute_shader, tCB_ComputeShader, b, 3);
CBUFFER(g_CB_animation_2D, tCB_Animation2D, b, 4);
CBUFFER(g_shared_animation3D_data, tAnimation3D_ComputeShader_Data, b, 5);
CBUFFER(g_CB_particle_system, tCB_ParticleSystem, b, 6);
CBUFFER(g_CB_noise, tCB_Noise, b, 7);

#ifdef __cplusplus

namespace core
{
	enum class eCBType
	{
		Global,	
		Camera,
		Material,	
		ComputeShader,	
		Animation2D,	
		Animation3D,
		ParticleSystem,
		Noise,
		END
	};
};

#endif


#endif
