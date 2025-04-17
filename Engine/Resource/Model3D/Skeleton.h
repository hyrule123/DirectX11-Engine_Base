#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Common.h"

#include "Engine/Util/Serialize/Serializable.h"

//MeshData에 종속된 클래스
namespace core
{
	class Animation3D_PlayData;
	class Animation3D_ComputeShader;
	class FBXLoader;
	class StructBuffer;
	class Animation3D;

	BASE_RESOURCE(Skeleton);
	class Skeleton final
		: public Resource
		, public Serializable_Binary
	{
		CLASS_NAME(Skeleton);
	public:
		Skeleton();
		virtual ~Skeleton();

		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual eResult serialize_binary(BinarySerializer* _ser) const override;
		virtual eResult deserialize_binary(const BinarySerializer* _ser) override;

		eResult create_from_FBX(FBXLoader* _fbxLoader);

		StructBuffer* get_bone_final_mat_buffer() { return m_final_matrix_buffer.get(); }
	
		//Animation 3D
		const std::vector<tMTBone>& get_bones() const { return m_vecBones; }
		UINT get_bone_count() const { return (UINT)m_vecBones.size(); }
		std::shared_ptr<StructBuffer> get_bone_offset_buffer() { return m_pBoneOffset; }	   // 각 뼈의 offset 행렬

		//반환타입 변수길이가 좀 김
		const std::unordered_map<std::string, std::shared_ptr<Animation3D>, Hasher_StringView, std::equal_to<>>& 
			get_animations() const { return m_animations; }

		std::shared_ptr<Animation3D> find_animation(const std::string_view _strAnimName);
		
		//return value: 인덱스 번호
		uint add_compute_queue(Animation3D_PlayData* _play_data) {
			ASSERT_DEBUG(_play_data, "play data가 없음.");
			m_compute_queue.push_back(_play_data);
			return (uint)m_compute_queue.size() - 1;
		}
		void compute_bone_final_matrix();
		void bind_bone_final_matrix_SRV();

		//애니메이션을 이동가능한지 확인하고, 이동시켜주는 함수
		bool copy_animation_from_other(const Skeleton& _other, const std::fs::path& _saveDir);
		int find_same_bone_index(const tMTBone& _other) const;

	private:
		void create_bone_offset_buffer();

	private:
		std::vector<tMTBone>							m_vecBones;

		// 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
		std::shared_ptr<StructBuffer>					m_pBoneOffset;	  

		std::unordered_map<std::string, std::shared_ptr<Animation3D>, Hasher_StringView, std::equal_to<>>	m_animations;

		//특정 프레임의 최종 행렬(여러개의 애니메이션을 받음)
		std::vector<Animation3D_PlayData*> m_compute_queue;
		std::unique_ptr<StructBuffer>	m_final_matrix_buffer;
		std::shared_ptr<Animation3D_ComputeShader>	m_compute_shader;
	};
}


