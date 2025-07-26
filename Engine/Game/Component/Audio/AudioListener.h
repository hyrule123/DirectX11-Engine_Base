#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
	class AudioListener 
		: public Component
	{
		CLASS_INFO(AudioListener, Component);
		BASE_COMPONENT(eComponentOrder::AudioListener);

		NO_SERIALIZE_BINARY;
		NO_SERIALIZE_JSON;
		
	public:
		AudioListener();

		//복사할 변수가 없으므로 기본 복사생성자( = default)로 충분함
		AudioListener(const AudioListener& _other) = default;
		CLONE_ABLE(AudioListener);

		virtual ~AudioListener();

		virtual void awake() override;
		virtual void update() override;
		virtual void final_update() override;
	private:
	};
}
