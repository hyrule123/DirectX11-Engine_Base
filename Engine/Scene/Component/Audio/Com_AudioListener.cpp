#include "Engine/Scene/Component/Audio/Com_AudioListener.h"

#include "Engine/Manager/AudioManager.h"

#include "Engine/Scene/GameObject.h"

#include "Engine/Scene/Component/Transform/Com_Transform.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	Com_AudioListener::Com_AudioListener()
	{

	}

	Com_AudioListener::~Com_AudioListener()
	{

	}

	eResult Com_AudioListener::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_AudioListener::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_AudioListener::Awake()
	{
	}

	void Com_AudioListener::Update()
	{
	}

	void Com_AudioListener::FinalUpdate()
	{
		const auto& tr = GetOwner()->GetComponent<Com_Transform>();
		float3 pos = tr->GetWorldPosition();

		float3 foward = tr->GetWorldDirection(eDirection::Forward);
		float3 up = tr->GetWorldDirection(eDirection::Up);

		float3 vel = { 0.0f, 0.0f, 0.0f };
		AudioManager::GetInst().Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

}
