#pragma once
#include "iAnimator.h"

namespace ehw
{
	class Animation3D;
	class Animation3D_PlayData;
	class Skeleton;
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
		virtual void InternalUpdate() override;
		virtual void FrameEnd() override;

		virtual bool IsPlaying() const;

		virtual bool AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func) override;

		std::shared_ptr<Animation3D_PlayData> CreateSharedAnimationData();
		inline void SetSharedAnimationData(const std::shared_ptr<Animation3D_PlayData>& _sharedData);


		bool Play(const std::string_view _strAnimName, float _blendTime = 0.f);

		//디버그용
		void PlayNext();

		//virtual void AddEvent(std::string_view _animName, int _frameIdx, const std::function<void()>& _func) {}

		//void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

		//StructBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer.get(); }
		//UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }


		virtual void BindData() override;
		virtual void UnBindData() override;


	private:
		std::shared_ptr<Animation3D_PlayData> m_sharedPlayData;
	};

	inline void Com_Animator3D::SetSharedAnimationData(const std::shared_ptr<Animation3D_PlayData>& _sharedData)
	{
		m_sharedPlayData = _sharedData;
	}
}


