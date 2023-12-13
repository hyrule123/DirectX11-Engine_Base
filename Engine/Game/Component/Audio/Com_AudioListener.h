#pragma once
#include "../iComponent.h"

namespace ehw
{
	class Com_AudioListener : public iComponent
	{
	public:
		Com_AudioListener();

		//복사할 변수가 없으므로 기본 복사생성자( = default)로 충분함
		Com_AudioListener(const Com_AudioListener& _other) = default;
		CLONE(Com_AudioListener);

		//저장할 변수가 없으므로 SaveJson, LoadJson 필요없음

		virtual ~Com_AudioListener();

		virtual void Awake() override;
		virtual void Update() override;
		virtual void InternalUpdate() override;
	private:
	};
}
