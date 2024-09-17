#pragma once
#include "Engine/Game/Component/Animator/Animator.h"

namespace ehw
{
	class Animation3D;
	class Animation3D_PlayData;
	class Skeleton;
	class Com_Animator3D :
		public Animator
	{
		CLASS_NAME(Com_Animator3D);
		
	public:
		Com_Animator3D();

		Com_Animator3D(const Com_Animator3D& _other);
		CLONE_ABLE(Com_Animator3D);

		virtual ~Com_Animator3D();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		//virtual void init() override;
		virtual void final_update() override;
		virtual void frame_end() override;

		virtual bool IsPlaying() const;

		virtual bool AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func) override;

		std::shared_ptr<Animation3D_PlayData> CreateSharedAnimationData();
		void SetSharedAnimationData(const std::shared_ptr<Animation3D_PlayData>& _sharedData) {
			m_sharedPlayData = _sharedData;
		}


		bool Play(const std::string_view _strAnimName, float _blendTime = 0.f);

		//디버그용
		void PlayNext();

		//virtual void AddEvent(std::string_view _animName, int _frameIdx, const std::function<void()>& _func) {}

		//void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

		//StructBuffer* GetFinalBoneMat() { return m_final_model_matrix_buffer.get(); }
		int get_bone_count();


		virtual void bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;


	private:
		std::shared_ptr<Animation3D_PlayData> m_sharedPlayData;
	};
}


