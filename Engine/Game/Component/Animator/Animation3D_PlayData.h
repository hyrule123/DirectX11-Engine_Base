#pragma once
#include "../../../Entity.h"

#include "../../../CommonStruct.h"

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
	public:
		Animation3D_PlayData();

		Animation3D_PlayData(const Animation3D_PlayData& _other);
		CLONE_ABLE(Animation3D_PlayData);

		virtual ~Animation3D_PlayData();

		bool InternalUpdate();
		void FrameEnd();
		bool BindData();
		void UnBindData();

		inline const std::shared_ptr<Animation3D>& GetCurrentAnimation() { return m_currentAnimation; }
		
		inline int GetPrevFrame() const { return m_PrevFrame; }
		inline int GetCurrentFrame() const { return m_Anim3DCBuffer.CurrentFrame; }
		int GetStartFrame() const;

		inline void UpdatePrevFrame() { m_PrevFrame = m_Anim3DCBuffer.CurrentFrame; }

		inline std::shared_ptr<Skeleton> GetSkeleton() { return m_skeleton; }
		void SetSkeleton(const std::shared_ptr<Skeleton> _skeleton);
		inline StructBuffer* GetBoneFinalMatrixStructBuffer() { return m_pBoneFinalMatBuffer.get(); }

		bool Play(const std::string_view _animationName, float _blendTime);
		void PlayNext();

	private:
		bool Play(const std::shared_ptr<Animation3D>& _animation, float _blendTime);

		bool CheckMesh();

	private:
		std::shared_ptr<Animation3D_ComputeShader>		m_animationComputeShader;
		std::shared_ptr<Skeleton>						m_skeleton;
		std::shared_ptr<Animation3D>					m_currentAnimation;
		std::shared_ptr<Animation3D>					m_nextAnimation;

		//std::vector<MATRIX>				m_vecFinalBoneMat;		// 텍스쳐에 전달할 최종 행렬정보

		//이전 프레임(현재 프레임은 아래 구조체에 저장되어 있음)
		int								m_PrevFrame;

		//GPU에 보낼 데이터 모음 구조체
		tCB_Animation3D					m_Anim3DCBuffer;

		//한 프레임별 시간 저장용 변수
		float							m_fClipUpdateTime;


		//전체 시간 길이
		float							m_fChangeTimeLength;

		//애니메이션 시작 이후 누적 시간
		float							m_fChangeTimeAccumulate;


		std::unique_ptr<StructBuffer>	m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬

		bool							m_bInternalUpdated;
		bool							m_bFinalMatrixUpdated;		// 최종행렬 연산 수행여부
	};
}

