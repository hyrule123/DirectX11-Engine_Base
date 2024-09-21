#pragma once
#include "Engine/Entity.h"

#include "Engine/Common.h"

//설명: Animation3D의 개별 플레이 관련 데이터를 저장하는 클래스
//참고 - class Animation3D: 변하지 않는 데이터
//하나의 Skeleton에 여러 개의 Mesh가 붙어있는 경우가 있음 -> 현재 상황 공유가 필요하기 때문에 만든 클래스


namespace ehw
{
	class Animation3D_ComputeShader;
	class Skeleton;
	class Animation3D;
	class StructBuffer;
	class Animation3D_PlayData
		: public Entity
	{
		CLASS_NAME(Animation3D_PlayData);
	public:
		Animation3D_PlayData();

		Animation3D_PlayData(const Animation3D_PlayData& _other);
		CLONE_ABLE(Animation3D_PlayData);

		virtual ~Animation3D_PlayData();

		
		bool is_pre_updated() const { return m_is_pre_updated; }

		//shared_data를 업데이트(컴퓨트쉐이더 연산 대기)
		bool pre_update();
		void update_final_matrix(uint _instance_id, StructBuffer* _final_matrix_buffer);
		void frame_end();
		uint get_matrix_instance_ID() const { return m_animation3D_data.instance_ID; }
		uint get_bone_count() const { return m_animation3D_data.BoneCount; }

		std::shared_ptr<Animation3D> get_current_animation() { return m_currentAnimation; }
		const tAnimation3D_ComputeShaderData& get_animation3D_data() const { return m_animation3D_data; }
		
		int get_prev_frame() const { return m_PrevFrame; }
		int get_current_frame() const { return m_animation3D_data.CurrentFrame; }
		int get_start_frame() const;

		void update_prev_frame() { m_PrevFrame = m_animation3D_data.CurrentFrame; }

		std::shared_ptr<Skeleton> get_skeleton() { return m_skeleton; }
		void set_skeleton(const std::shared_ptr<Skeleton> _skeleton);

		bool is_playing() const { return nullptr != m_currentAnimation; }
		bool Play(const std::string_view _animationName, float _blendTime);
		void PlayNext();

	private:
		bool Play(const std::shared_ptr<Animation3D>& _animation, float _blendTime);

	private:
		std::shared_ptr<Skeleton>						m_skeleton;
		std::shared_ptr<Animation3D>					m_currentAnimation;
		std::shared_ptr<Animation3D>					m_nextAnimation;

		//최종행렬을 계산해줄 컴퓨트쉐이더
		std::shared_ptr<Animation3D_ComputeShader>		m_compute_shader;

		//이전 프레임(현재 프레임은 아래 구조체에 저장되어 있음)
		int								m_PrevFrame;
		
		//GPU에 보낼 데이터 모음 구조체
		tAnimation3D_ComputeShaderData					m_animation3D_data;

		//한 프레임별 시간 저장용 변수
		float							m_fClipUpdateTime;

		//전체 시간 길이
		float							m_fChangeTimeLength;

		//애니메이션 시작 이후 누적 시간
		float							m_fChangeTimeAccumulate;

		
		bool							m_is_pre_updated;
	};
}

