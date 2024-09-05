#pragma once
#include "Engine/Game/Component/Component.h"

namespace ehw
{
	class Com_AudioListener 
		: public Component<Com_AudioListener, eComponentCategory::AudioListener>
	{
		CLASS_NAME(Com_AudioListener);
		
	public:
		Com_AudioListener();

		//복사할 변수가 없으므로 기본 복사생성자( = default)로 충분함
		Com_AudioListener(const Com_AudioListener& _other) = default;
		CLONE_ABLE(Com_AudioListener);

		virtual ~Com_AudioListener();

		virtual eResult Serialize_Json(JsonSerializer* _ser) const override;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override;

		virtual void Awake() override;
		virtual void Update() override;
		virtual void FinalUpdate() override;
	private:
	};
}
