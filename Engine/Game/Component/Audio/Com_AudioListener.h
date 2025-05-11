#pragma once
#include "Engine/Game/Component/Component.h"

namespace core
{
	class Com_AudioListener 
		: public Component
	{
		CLASS_INFO(Com_AudioListener, Component);
		
	public:
		Com_AudioListener();

		//복사할 변수가 없으므로 기본 복사생성자( = default)로 충분함
		Com_AudioListener(const Com_AudioListener& _other) = default;
		CLONE_ABLE(Com_AudioListener);

		virtual ~Com_AudioListener();

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		virtual void Awake() override;
		virtual void update() override;
		virtual void final_update() override;
	private:
	};
}
