#include "AudioListener.h"

#include <Engine/Manager/AudioManager.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Transform.h>


#include <Fmod/fmod.hpp>
#include <Fmod/fmod_studio.hpp>
#include <Fmod/fmod_common.h>
#include <Fmod/fmod_codec.h>

namespace core
{
	AudioListener::AudioListener()
		: Super(s_component_order)
	{
	}

	AudioListener::~AudioListener()
	{
	}

	eResult AudioListener::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult AudioListener::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void AudioListener::awake()
	{
	}

	void AudioListener::update()
	{
	}

	void AudioListener::final_update()
	{
		const auto& tr = get_owner()->get_component<Transform>();
		float3 pos = tr->get_world_position();

		float3 foward = tr->get_world_direction(eDirection::Forward);
		float3 up = tr->get_world_direction(eDirection::Up);

		float3 vel = { 0.0f, 0.0f, 0.0f };
		AudioManager::get_inst().set_3D_listener_attrib(&pos, &vel, &foward, &up);
	}

}
