#ifndef COMMON_REGISTER_HLSLI
#define COMMON_REGISTER_HLSLI

#include "Engine/DefaultShader/Common_struct.hlsli"

#ifdef __cplusplus
REGISTER_DECLARE(NONE, t, -1);
REGISTER_DECLARE(NONE, u, -1);
#endif

//t 버퍼

//GPU Initalize Setting: 첫 실행 시 1회 계산 후 127번 버퍼에 계속 바인딩됨
SBUFFER(gInitSetting, tGPUInitSetting, t, 127);

//Material Default Texture
TEXTURE2D(g_is_texture_0, t, 0);
TEXTURE2D(g_is_texture_1, t, 1);
TEXTURE2D(g_is_texture_2, t, 2);
TEXTURE2D(g_is_texture_3, t, 3);
TEXTURE2D(g_is_texture_4, t, 4);
TEXTURE2D(g_is_texture_5, t, 5);
TEXTURE2D(g_is_texture_6, t, 6);
TEXTURE2D(g_is_texture_7, t, 7);


//모델링이 가지고있는 텍스처
TEXTURE2D(g_diffuse_texture, t, 0);
TEXTURE2D(g_normal_texture, t, 1);
TEXTURE2D(g_specular_texture, t, 2);
TEXTURE2D(g_emissive_texture, t, 3);
TEXTURE2D(g_roughness_metalic_texture, t, 4);
#define IsAlbedoTex bTex_0
#define IsNormalTex bTex_1
#define IsSpecularTex bTex_2
#define IsEmissiveTex bTex_3
#define IsRMTTex bTex_4


//Deffered + Light 렌더타겟을 "샘플링용 텍스처"로 바인딩할때
TEXTURE2D(g_diffuse_rendertarget, t, 0);
TEXTURE2D(g_specular_rendertarget, t, 1);
TEXTURE2D(g_normal_rendertarget, t, 2);
TEXTURE2D(g_view_position_rendertarget, t, 3);
TEXTURE2D(g_emissive_rendertarget, t, 4);
TEXTURE2D(g_roughness_metalic_rendertarget, t, 5);
TEXTURE2D(g_diffuse_light_rendertarget, t, 6);
TEXTURE2D(g_specular_light_rendertarget, t, 7);

SBUFFER(g_transforms, tTransform, t, 10);
SBUFFER(g_default_3D_mtrl_instancing_buffer, tDefault3DMtrl_InstancingData, t, 11);

//Bone Matrices(Animation 3D)
//현재 재생중인 애니메이션의 키프레임 데이터
SBUFFER(g_FrameTransArray, tAnimKeyframeTranslation, t, 16);
//(애니메이션 변경중일 경우) 다음 애니메이션의 키프레임 데이터
SBUFFER(g_ChangeFrameTransArray, tAnimKeyframeTranslation, t, 17);
SBUFFER(g_BoneOffsetArray, Matrix, t, 18);

//최종 행렬이 저장되는 구조화버퍼
SBUFFER(g_FinalBoneMatrixArray, Matrix, t, 19);
SBUFFER_RW(g_FinalBoneMatrixArrayRW, Matrix, u, 1);

//본 인스턴싱용
SBUFFER_RW(g_BoneSocketMatrixArray, tOutputBoneInfo, u, 2);
SBUFFER_RW(g_InstancingBoneMatrixArray, Matrix, u, 3);

// noise
TEXTURE2D(NoiseTexture, t, 16);

// postProcess
TEXTURE2D(postProcessTexture, t, 60);
TEXTURE2D(guiGameTexture, t, 61);


#endif
