#include "Engine/Game/Component/Audio/Com_AudioListener.h"

#include "Engine/Manager/AudioManager.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Game/Component/Transform.h"


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace ehw
{
	Com_AudioListener::Com_AudioListener()
		: Component(Com_AudioListener::concrete_class_name)
	{
	}

	Com_AudioListener::~Com_AudioListener()
	{
	}

	eResult Com_AudioListener::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_AudioListener::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_AudioListener::Awake()
	{
	}

	void Com_AudioListener::update()
	{
	}

	void Com_AudioListener::final_update()
	{
		const auto& tr = gameObject()->GetComponent<Transform>();
		float3 pos = tr->get_world_position();

		float3 foward = tr->get_world_direction(eDirection::Forward);
		float3 up = tr->get_world_direction(eDirection::Up);

		float3 vel = { 0.0f, 0.0f, 0.0f };
		AudioManager::get_inst().Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

}
