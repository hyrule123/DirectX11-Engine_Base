#pragma once
#include <Engine/Resource/Shader/ComputeShader.h>

#include <Engine/DefaultShader/Common_struct.hlsli>

//설명: 3D 애니메이션을 처리하기 위한 컴퓨트쉐이더
//사용법
//* tAnimation3D_ComputeShader_Desc 구조체에 필요한 내용을 넣어서 전달.

namespace core
{
	class StructBuffer;
    class Animation3D_ComputeShader :
        public ComputeShader
    {
		CLASS_INFO(Animation3D_ComputeShader, ComputeShader);
		REGISTER_FACTORY(Animation3D_ComputeShader);
	public:
		Animation3D_ComputeShader();
		virtual ~Animation3D_ComputeShader();

		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual bool bind_buffer_to_GPU_register();
		virtual void unbind_buffer_from_GPU_register();

		struct Desc
		{
			const struct tAnimation3D_ComputeShader_Data* shared_animation_data{};
			class StructBuffer* current_animation_key_frame_buffer{};
			class StructBuffer* next_animation_keyframe_buffer{};
			class StructBuffer* bone_offset_matrix_buffer{};
			class StructBuffer* final_bone_translation_matrix_buffer{};
		};
		bool on_execute(const Animation3D_ComputeShader::Desc& _desc);

	private:
		Desc m_desc;
    };
}


