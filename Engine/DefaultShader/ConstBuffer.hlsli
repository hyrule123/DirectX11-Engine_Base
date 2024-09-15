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
CBUFFER(CB_Global, tCB_Global, b, 0);
CBUFFER(g_CB_camera, tCamera, b, 1);
CBUFFER(CB_MaterialData, tSharedMaterialData, b, 2);
CBUFFER(CB_ComputeShader, tCB_ComputeShader, b, 3);
CBUFFER(CB_Animation2D, tCB_Animation2D, b, 4);
CBUFFER(CB_Animation3D, tCB_Animation3D, b, 5);
CBUFFER(CB_ParticleSystem, tCB_ParticleSystem, b, 6);
CBUFFER(CB_Noise, tCB_Noise, b, 7);

#ifdef __cplusplus

namespace ehw
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
