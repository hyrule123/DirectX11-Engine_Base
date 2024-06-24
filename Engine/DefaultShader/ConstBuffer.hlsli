#ifndef CONST_BUFFER_HLSLI
#define CONST_BUFFER_HLSLI

#include "Engine/DefaultShader/CommonStruct.hlsli"


//////////////////////////////////
//새로운 상수버퍼 생성시 해줘야 할것
//구조체 선언
//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호) 작성
//아래 C++ enum class에 해당 번호 추가
//RenderMgr에서 상수버퍼 추가
/////////////////////////////////


//CBUFFER(버퍼이름, 사용할 구조체이름, 버퍼타입, 버퍼번호)
CBUFFER(CB_Global, tCB_Global, b, 0);
CBUFFER(CB_Transform, tCB_Transform, b, 1);
CBUFFER(CB_MaterialData, tCB_MaterialData, b, 2);
CBUFFER(CB_ComputeShader, tCB_ComputeShader, b, 3);
CBUFFER(CB_Grid, tCB_Grid, b, 4);
CBUFFER(CB_Animation2D, tCB_Animation2D, b, 5);
CBUFFER(CB_NumberOfLight, tCB_NumberOfLight, b, 6);
CBUFFER(CB_ParticleSystem, tCB_ParticleSystem, b, 7);
CBUFFER(CB_Noise, tCB_Noise, b, 8);
CBUFFER(CB_Animation3D, tCB_Animation3D, b, 9);
CBUFFER(CB_CustomData, tCB_CustomData, b, 10);


#ifdef __cplusplus

namespace ehw
{
	enum class eCBType
	{
		Global,	
		Transform,	
		Material,	
		ComputeShader,	
		Grid,		
		Animation2D,	
		numberOfLight,
		ParticleSystem,
		Noise,
		Animation3D,
		CustomData,
		END
	};
};

#endif


#endif
