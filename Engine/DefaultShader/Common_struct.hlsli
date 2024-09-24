#ifndef COMMON_STRUCT_HLSLI
#define COMMON_STRUCT_HLSLI
#include "Engine/DefaultShader/Common_define.hlsli"

//C++와 공동으로 사용하는 구조체 모음
struct alignas(16)  tGPUInitSetting
{
	BOOL bCPULittleEndian;
	BOOL bGPULittleEndian;
	uint GPUEndianessTestVar;
	uint Padding_InitSetting;
	
	uint u8BitPartInU32Pack[4];
	uint u8BitShiftInU32Pack[4];

	uint u16BitPartInU32Pack[2];
	uint u16BitShiftInU32Pack[2];
	
	uint4 debugVar;
};

struct alignas(16)  tCB_Global
{
	uint2 uResolution;
	float2 fResolution;
	
	float DeltaTime;
	uint AccFramePrev;
	uint AccFrame;	//여태까지 누적된 프레임 갯수
	int PadGlobal;
};

struct alignas(16) tTransform
{
	MATRIX World;
	MATRIX InverseWorld;
	MATRIX WorldView;
	MATRIX WVP;
};

struct alignas(16)  tCamera
{
	MATRIX view;
	MATRIX inverse_view;
	MATRIX projection;
};

struct alignas(16) tSharedMaterialData
{
	BOOL bTex_0;
	BOOL bTex_1;
	BOOL bTex_2;
	BOOL bTex_3;
	BOOL bTex_4;
	BOOL bTex_5;
	BOOL bTex_6;
	BOOL bTex_7;
	
	float4 Diff;
	float4 Spec;
	float4 Amb;
	float4 Emv;
};

struct alignas(16)  tDefault3DMtrl_InstancingData
{
	// 3D Animation 정보
	BOOL bAnim;
	uint BoneCount;
	uint model_instance_ID;
	int Padding_Material;
};

struct alignas(16)   tCB_ComputeShader
{
#ifdef __cplusplus
	const uint3 ThreadsPerGroup;
#else
	uint3 ThreadsPerGroup;
#endif
	
	uint Padding_CS1;
	
	uint3 NumGroup;
	uint Padding_CS2;
	
	uint3 TotalDataCount;
	uint Padding_CS3;
};

struct alignas(16)  tCB_Grid
{
	float4 cameraPosition;
	float2 cameraScale;
	float2 resolution;
};


struct alignas(16)  tCB_Animation2D
{
	float2 leftTop;
	float2 spriteSize;
	float2 offset;
	float2 atlasSize;

	uint animationType;
	float3 pad_Anim2D;
};


struct alignas(16)  tCB_NumberOfLight
{
	uint numberOfLight;
	uint indexOfLight;
	
	float2 pad3;
};


struct alignas(16)  tCB_ParticleSystem
{
	float4 worldPosition;
	float4 startColor;
	float4 startSize;
    
	uint maxParticles;
	uint simulationSpace;
	float radius;
	float startSpeed;
    
	float startLifeTime;
	float deltaTime;
	float elapsedTime; //누적시간
	int LightPad;
};


struct alignas(16)  tCB_Noise
{
	float4 NoiseSize;
	
	float NoiseTime;
	float3 NoisePad;
};

struct alignas(16)  tAnimation3D_ComputeShaderData
{
	uint		BoneCount;		//본 갯수
	//Instancing 관련
	uint		model_inst_ID; //내가 몇번째 인스턴스 인지
	float2		pad;
	
	uint		CurrentFrame;	//현재 프레임
	uint		NextFrame;		//다음 프레임
	float		FrameRatio;		//프레임 진행 비율
	uint		FrameLength;	//전체 프레임 장수
	
	//Animation Blending 관련
	BOOL		bChangingAnim;
	float		ChangeRatio;
	
	uint		ChangeFrameLength;
	uint		ChangeFrameIdx;
};

struct alignas(16)  tCB_CustomData
{
	int int_0;
	int int_1;
	int int_2;
	int int_3;
    
	float float_0;
	float float_1;
	float float_2;
	float float_3;
    
	float2 float2_0;
	float2 float2_1;
	float2 float2_2;
	float2 float2_3;

	float4 float4_0;
	float4 float4_1;
	float4 flaot4_2;
	float4 float4_3;

    MATRIX mat_0;
    MATRIX mat_1;
    MATRIX mat_2;
    MATRIX mat_3;
};


struct alignas(16)  tLightColor
{
	float4 diffuse;
	float4 specular;
	float4 ambient;
};



struct alignas(16) tAnimKeyframeTranslation
{
	float4 Pos;
	float4 Scale;
	float4 RotQuat;
};

struct alignas(16)  tSkinningInfo
{
	float3 Pos;
	float3 Tangent;
	float3 Binormal;
	float3 Normal;
};

struct alignas(16) tOutputBoneInfo
{
	MATRIX matBone;
	float3 Pos;
	float Empty1;
	float3 Scale;
	float Empty2;
	float4 RotQuat;
};




#endif//COMMON_STRUCT_HLSLI
