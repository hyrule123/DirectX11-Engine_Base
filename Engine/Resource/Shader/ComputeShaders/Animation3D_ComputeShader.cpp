#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"

#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/DefaultShader/Common_register.hlsli"

#if defined (_WIN64) && (_DEBUG)
#include "Engine/CompiledShaderHeader/Animation3D_CS_x64_Debug.h"
#else
#error 헤더 include 해주세요
#endif

namespace ehw
{
	Animation3D_ComputeShader::Animation3D_ComputeShader()
		: ComputeShader(Animation3D_ComputeShader::concrete_class_name, uint3(256u, 1u, 1u))
		, m_desc()
	{ 
	}
	Animation3D_ComputeShader::~Animation3D_ComputeShader()
	{
	}

	eResult Animation3D_ComputeShader::load_from_file(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		return ComputeShader::compile_from_byte_code(Animation3D_CS, sizeof(Animation3D_CS));
	}


	bool Animation3D_ComputeShader::bind_buffer_to_GPU_register()
	{
		//참고 - NextAnimKeyFrameBuffer은 없을수도 있음
		if (false == 
			(
				m_desc.shared_animation_data 
				|| m_desc.current_animation_key_frame_buffer 
				|| m_desc.bone_offset_matrix_buffer 
				|| m_desc.final_bone_translation_matrix_buffer))
			return false;

		// 구조화버퍼 전달
		m_desc.current_animation_key_frame_buffer->bind_data_SRV(GPU::Register::t::g_FrameTransArray, eShaderStageFlag::Compute);// t16;

		//다음 애니메이션 정보가 있는 경우 바인드
		if (m_desc.next_animation_keyframe_buffer)
		{
			m_desc.next_animation_keyframe_buffer->bind_data_SRV(GPU::Register::t::g_ChangeFrameTransArray, eShaderStageFlag::Compute);//t17
		}

		//본의 오프셋 행렬 버퍼 바인드
		m_desc.bone_offset_matrix_buffer->bind_data_SRV(GPU::Register::t::g_BoneOffsetArray, eShaderStageFlag::Compute);// t18

		//본의 최종 행렬정보를 저장할 UAV 바인드
		m_desc.final_bone_translation_matrix_buffer->bind_buffer_to_UAV(GPU::Register::u::g_FinalBoneMatrixArrayRW); // u0
		
		//애니메이션 3D 상수버퍼 바인드
		ConstBuffer* pAnimCB = RenderManager::get_inst().GetConstBuffer(eCBType::Animation3D);
		pAnimCB->set_data(m_desc.shared_animation_data);
		pAnimCB->bind_buffer_to_GPU_register();

		//데이터 갯수 계산
		uint3 DataCounts{};
		DataCounts.x = m_desc.shared_animation_data->BoneCount;
		DataCounts.y = 1;
		DataCounts.z = 1;
		
		//데이터 그룹 갯수 계산
		calculate_group_count(DataCounts);

		return true;
	}


	void Animation3D_ComputeShader::unbind_buffer_from_GPU_register()
	{
		// 전달한 구조화버퍼 전부 UnBind 하고 비워줌
		m_desc.current_animation_key_frame_buffer->unbind_buffer();
		if (m_desc.next_animation_keyframe_buffer)
		{
			m_desc.next_animation_keyframe_buffer->unbind_buffer();
		}
		m_desc.bone_offset_matrix_buffer->unbind_buffer();
		m_desc.final_bone_translation_matrix_buffer->unbind_buffer();
		
		m_desc = {};
	}

	bool Animation3D_ComputeShader::on_execute(const Animation3D_ComputeShader::Desc& _desc)
	{
		m_desc = _desc;
		return ComputeShader::on_execute();
	}
}
