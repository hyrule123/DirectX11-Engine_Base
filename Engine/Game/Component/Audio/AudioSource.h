#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
	class AudioClip;
	class AudioSource 
		: public Component
	{
		CLASS_INFO(AudioSource, Component);
		BASE_COMPONENT(eComponentOrder::AudioSource);

		//다 같이 공유하는 '리소스' 포인터만 변수로 가지고 있으므로 기본 복사 생성자로도 충분하다.
		CLONE_ABLE(AudioSource);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;

	public:
		AudioSource();
		AudioSource(const AudioSource& _other) = default;

		virtual ~AudioSource();

		virtual void final_update() override;

		void play();
		void stop();
		void set_loop(bool _loop);

		void SetClip(s_ptr<AudioClip> _clip) { m_audio_clip = _clip; }
		s_ptr<AudioClip> GetClip() const { return m_audio_clip; }

	private:
		s_ptr<AudioClip> m_audio_clip;
	};
}
