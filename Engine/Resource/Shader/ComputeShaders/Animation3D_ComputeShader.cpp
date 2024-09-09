#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Common_register.hlsli"

#if defined (_WIN64) && (_DEBUG)
#include "Engine/CompiledShaderHeader/CS_Animation3D_x64_Debug.h"
#else
#error 헤더 include 해주세요
#endif

namespace ehw
{
	Animation3D_ComputeShader::Animation3D_ComputeShader()
		: ComputeShader(Animation3D_ComputeShader::concrete_name, uint3(256u, 1u, 1u))
		, m_desc()
	{ 
	}
	Animation3D_ComputeShader::~Animation3D_ComputeShader()
	{
	}

	eResult Animation3D_ComputeShader::load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
	{
		return ComputeShader::CreateByHeader(CS_Animation3D, sizeof(CS_Animation3D));
	}


	bool Animation3D_ComputeShader::bind_data()
	{
		//참고 - NextAnimKeyFrameBuffer은 없을수도 있음
		if (false == 
			(
				m_desc.Anim3DData 
				|| m_desc.CurrentAnimKeyFrameBuffer 
				|| m_desc.BoneOffsetMatrixBuffer 
				|| m_desc.FinalBoneTranslationMatrixBuffer))
			return false;

		// 구조화버퍼 전달
		m_desc.CurrentAnimKeyFrameBuffer->BindDataSRV(GPU::Register::t::g_FrameTransArray, eShaderStageFlag::Compute);// t16;

		//다음 애니메이션 정보가 있는 경우 바인드
		if (m_desc.NextAnimKeyFrameBuffer)
		{
			m_desc.NextAnimKeyFrameBuffer->BindDataSRV(GPU::Register::t::g_ChangeFrameTransArray, eShaderStageFlag::Compute);//t17
		}

		//본의 오프셋 행렬 버퍼 바인드
		m_desc.BoneOffsetMatrixBuffer->BindDataSRV(GPU::Register::t::g_BoneOffsetArray, eShaderStageFlag::Compute);// t18

		//본의 최종 행렬정보를 저장할 UAV 바인드
		m_desc.FinalBoneTranslationMatrixBuffer->BindDataUAV(GPU::Register::u::g_FinalBoneMatrixArrayRW); // u0
		
		//애니메이션 3D 상수버퍼 바인드
		ConstBuffer* pAnimCB = RenderManager::GetInst().GetConstBuffer(eCBType::Animation3D);
		pAnimCB->SetData(m_desc.Anim3DData);
		pAnimCB->bind_data();

		//데이터 갯수 계산
		uint3 DataCounts{};
		DataCounts.x = m_desc.Anim3DData->BoneCount;
		DataCounts.y = 1;
		DataCounts.z = 1;
		
		//데이터 그룹 갯수 계산
		CalculateGroupCount(DataCounts);

		return true;
	}


	void Animation3D_ComputeShader::UnBindData()
	{
		// 전달한 구조화버퍼 전부 UnBind 하고 비워줌
		m_desc.CurrentAnimKeyFrameBuffer->unbind_data();
		if (m_desc.NextAnimKeyFrameBuffer)
		{
			m_desc.NextAnimKeyFrameBuffer->unbind_data();
		}
		m_desc.BoneOffsetMatrixBuffer->unbind_data();
		m_desc.FinalBoneTranslationMatrixBuffer->unbind_data();
		
		memset(&m_desc, 0, sizeof(Desc));
	}
}
