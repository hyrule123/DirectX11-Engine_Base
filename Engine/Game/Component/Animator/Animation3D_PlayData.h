#pragma once
#include "Engine/Game/Entity.h"

#include "Engine/Common.h"
#include "Engine/GPU/CommonGPU.h"

//설명: Animation3D의 개별 플레이 관련 데이터를 저장하는 클래스
//참고 - class Animation3D: 변하지 않는 데이터
//하나의 Skeleton에 여러 개의 Mesh가 붙어있는 경우가 있음 -> 현재 상황 공유가 필요하기 때문에 만든 클래스

/*
흐름: pre_update에서 현재 애니메이션이 몇 프레임인지 계산
계산 후 자신의 주소를 skeleton에 등록
skeleton에서는 render 단계에서 저장된 주소들에 대해 일괄 
*/

namespace core
{
	class Skeleton;
	class Animation3D;
	class StructBuffer;
	class Animation3D_PlayData
		: public Entity
	{
		CLASS_INFO(Animation3D_PlayData, Entity);
		CLONE_ABLE(Animation3D_PlayData);
	public:
		Animation3D_PlayData();
		Animation3D_PlayData(const Animation3D_PlayData& _other);

		virtual ~Animation3D_PlayData();

		bool is_pre_updated() const { return m_is_pre_updated; }
		bool pre_update();

		bool is_playing() const { return nullptr != m_currentAnimation; }
		bool play(const std::string_view _animationName, float _blendTime);
		void play_next();

		void bind_computed_final_bone_matrix();

		//skeleton쪽에서 호출. 자신의 final matrix를 업데이트 한다.
		void frame_end();
		uint get_bone_count() const { return m_animation3D_data.BoneCount; }
		uint get_model_inst_ID() const { return m_animation3D_data.model_inst_ID; }

		s_ptr<Animation3D> get_current_animation() { return m_currentAnimation; }
		s_ptr<Animation3D> get_next_animation() { return m_nextAnimation; }
		const tAnimation3D_ComputeShader_Data& get_compute_shader_data() const { return m_animation3D_data; }
		
		int get_prev_frame() const { return m_PrevFrame; }
		int get_current_frame() const { return m_animation3D_data.CurrentFrame; }
		int get_start_frame() const;

		void update_prev_frame() { m_PrevFrame = m_animation3D_data.CurrentFrame; }

		s_ptr<Skeleton> get_skeleton() { return m_skeleton; }
		void set_skeleton(const s_ptr<Skeleton> _skeleton);
		


	private:
		bool play_internal(const s_ptr<Animation3D>& _animation, float _blendTime);

	private:
		s_ptr<Skeleton>						m_skeleton;
		s_ptr<Animation3D>					m_currentAnimation;
		s_ptr<Animation3D>					m_nextAnimation;

		//이전 프레임(현재 프레임은 아래 구조체에 저장되어 있음)
		int								m_PrevFrame;
		
		//GPU에 보낼 데이터 모음 구조체
		tAnimation3D_ComputeShader_Data					m_animation3D_data;

		//한 프레임별 시간 저장용 변수
		double							m_fClipUpdateTime;

		//전체 시간 길이
		double							m_fChangeTimeLength;

		//애니메이션 시작 이후 누적 시간
		double							m_fChangeTimeAccumulate;
		
		bool							m_is_pre_updated;
	};
}

