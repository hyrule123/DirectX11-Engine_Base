#pragma once
#include "Engine/Game/Component/Component.h"

namespace ehw
{
	class AudioClip;
	class Com_AudioSource 
		: public Component<Com_AudioSource, eComponentCategory::AudioSource>
	{
		REGISTER_CLASS_INSTANCE_ABLE(Com_AudioSource);
		
	public:
		Com_AudioSource();

		//복사할 변수가 있지만,
		//다 같이 공유하는 '리소스' 포인터이기 때문에 기본 복사 생성자로도 충분하다.
		Com_AudioSource(const Com_AudioSource& _other) = default;
		CLONE_ABLE(Com_AudioSource);

		virtual ~Com_AudioSource();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void FinalUpdate() override;

		void Play();
		void Stop();
		void SetLoop(bool _loop);

		void SetClip(std::shared_ptr<AudioClip> _clip) { mAudioClip = _clip; }
		std::shared_ptr<AudioClip> GetClip() const { return mAudioClip; }

	private:
		std::shared_ptr<AudioClip> mAudioClip;
	};
}
