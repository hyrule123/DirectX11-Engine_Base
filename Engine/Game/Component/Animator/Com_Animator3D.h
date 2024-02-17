#pragma once
#include "iAnimator.h"



#include "../../../CommonStruct.h"

#include <fbxsdk/fbxsdk.h>

namespace ehw
{
	class StructBuffer;
	class Mesh;
	class Skeleton;
	class Animation3D;
	class Com_Animator3D :
		public iAnimator
	{
	public:
		Com_Animator3D();

		Com_Animator3D(const Com_Animator3D& _other);
		CLONE_ABLE(Com_Animator3D);

		virtual ~Com_Animator3D();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void Init() override;
		virtual void Update() {};
		virtual void InternalUpdate() override;

		virtual bool IsPlaying() const { return (nullptr != mCurrentAnim); }

		virtual bool AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func) override;

		void SetSkeleton(std::shared_ptr<Skeleton> _pSkeleton);
		std::shared_ptr<const Skeleton> GetSkeleton() const { return mSkeleton; }
		bool Play(const std::string_view _strAnimName, float _blendTime = 0.f);

		//디버그용
		void PlayNext();

		//virtual void AddEvent(std::string_view _animName, int _frameIdx, const std::function<void()>& _func) {}

		//void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

		StructBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer.get(); }
		//UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }


		virtual void BindData() override;
		virtual void UnBindData() override;
	

	private:
		bool Play(std::shared_ptr<Animation3D> _anim, float _blendTime);

		bool CheckMesh();

    private:
		std::shared_ptr<Skeleton>						mSkeleton;
		std::shared_ptr<Animation3D>					mCurrentAnim;
		std::shared_ptr<Animation3D>					mNextAnim;

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
        bool							m_bFinalMatUpdate;		// 최종행렬 연산 수행여부
	};
}


