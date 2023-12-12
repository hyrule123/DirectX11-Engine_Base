#include "../../PCH_Engine.h"
#include "../../Resource/ComputeShaders/Animation3DShader.h"



#include "../../Manager/RenderManager.h"

#include "../../GPU/ConstBuffer.h"
#include "../../GPU/StructBuffer.h"

#include "../../DefaultShader/Resource.hlsli"

namespace ehw
{
	Animation3DShader::Animation3DShader()
		: iComputeShader(typeid(Animation3DShader), uint3(256u, 1u, 1u))
		, mDesc()
	{ 
	}
	Animation3DShader::~Animation3DShader()
	{
	}


	bool Animation3DShader::BindData()
	{
		if (false == 
			(
				mDesc.Anim3DData 
				|| mDesc.CurrentAnimKeyFrameBuffer 
				|| mDesc.BoneOffsetMatrixBuffer 
				|| mDesc.FinalBoneTranslationMatrixBuffer))
			return false;

		// 구조화버퍼 전달
		mDesc.CurrentAnimKeyFrameBuffer->BindDataSRV(Register_t_g_FrameTransArray, eShaderStageFlag::Compute);// t16;

		//다음 애니메이션 정보가 있는 경우 바인드
		if (mDesc.NextAnimKeyFrameBuffer)
		{
			mDesc.NextAnimKeyFrameBuffer->BindDataSRV(Register_t_g_ChangeFrameTransArray, eShaderStageFlag::Compute);//t17
		}

		//본의 오프셋 행렬 버퍼 바인드
		mDesc.BoneOffsetMatrixBuffer->BindDataSRV(Register_t_g_BoneOffsetArray, eShaderStageFlag::Compute);// t18

		//본의 최종 행렬정보를 저장할 UAV 바인드
		mDesc.FinalBoneTranslationMatrixBuffer->BindDataUAV(Register_u_g_FinalBoneMatrixArrayRW); // u0
		
		//애니메이션 3D 상수버퍼 바인드
		ConstBuffer* pAnimCB = RenderManager::GetConstBuffer(eCBType::Animation3D);
		pAnimCB->SetData(mDesc.Anim3DData);
		pAnimCB->BindData();

		//데이터 갯수 계산
		uint3 DataCounts{};
		DataCounts.x = mDesc.Anim3DData->BoneCount;
		DataCounts.y = 1;
		DataCounts.z = 1;
		
		//데이터 그룹 갯수 계산
		CalculateGroupCount(DataCounts);

		return true;
	}


	void Animation3DShader::UnBindData()
	{
		// 전달한 구조화버퍼 전부 UnBind 하고 비워줌
		mDesc.CurrentAnimKeyFrameBuffer->UnBindData();
		if (mDesc.NextAnimKeyFrameBuffer)
		{
			mDesc.NextAnimKeyFrameBuffer->UnBindData();
		}
		mDesc.BoneOffsetMatrixBuffer->UnBindData();
		mDesc.FinalBoneTranslationMatrixBuffer->UnBindData();
		
		memset(&mDesc, 0, sizeof(Desc));
	}
}
