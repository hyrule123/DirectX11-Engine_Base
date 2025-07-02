#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
	class AudioClip;
	class Com_AudioSource 
		: public Component
	{
		CLASS_INFO(Com_AudioSource, Component);
		BASE_COMPONENT(eComponentOrder::AudioSource);

		//다 같이 공유하는 '리소스' 포인터만 변수로 가지고 있으므로 기본 복사 생성자로도 충분하다.
		CLONE_ABLE(Com_AudioSource);
	public:
		Com_AudioSource();
		Com_AudioSource(const Com_AudioSource& _other) = default;

		virtual ~Com_AudioSource();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		virtual void final_update() override;

		void Play();
		void Stop();
		void SetLoop(bool _loop);

		void SetClip(s_ptr<AudioClip> _clip) { mAudioClip = _clip; }
		s_ptr<AudioClip> GetClip() const { return mAudioClip; }

	private:
		s_ptr<AudioClip> mAudioClip;
	};
}
